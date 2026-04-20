#include "Database.h"
#include <iostream>

Database& Database::getInstance()
{
    static Database instance;
    return instance;
}

// =========================
// OPEN DB
// =========================
#include <QCoreApplication>
#include <QDir>


bool Database::open()
{
    QString dbPath = QCoreApplication::applicationDirPath()
        + "/rubiksdb.db";

    std::cout << "DB PATH USED: " << dbPath.toStdString() << "\n";

    db = nullptr;

    int rc = sqlite3_open(dbPath.toStdString().c_str(), &db);

    if (rc != SQLITE_OK || !db)
    {
        std::cout << "? DB OPEN FAILED: "
            << (db ? sqlite3_errmsg(db) : "null db") << "\n";
        db = nullptr;
        return false;
    }

    std::cout << "? DB OPEN SUCCESS\n";
    return true;
}

// =========================
// CLOSE DB
// =========================
void Database::close()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

// =========================
// CREATE TABLES
// =========================
void Database::initTables()
{
    if (!db) return;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS session ("
        "id INTEGER PRIMARY KEY,"
        "current_face INTEGER,"
        "cube_state TEXT);";

    char* err = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK)
    {
        std::cout << "? Table init failed: " << err << "\n";
        sqlite3_free(err);
    }
}

// =========================
// SAVE SESSION
// =========================
void Database::saveSession(int face, const std::string& cubeState)
{
    if (!db)
    {
        std::cout << "? DB not open (saveSession)\n";
        return;
    }

    const char* sql =
        "INSERT OR REPLACE INTO session (id, current_face, cube_state) "
        "VALUES (1, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cout << "? Prepare failed: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, face);
    sqlite3_bind_text(stmt, 2, cubeState.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cout << "? Save failed: " << sqlite3_errmsg(db) << "\n";
    }
    else
    {
        std::cout << "? Session saved (face=" << face << ")\n";
    }

    sqlite3_finalize(stmt);

    // ?? FORCE FLUSH (IMPORTANT)
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

// =========================
// LOAD SESSION
// =========================
bool Database::loadSession(int& face, std::string& cubeState)
{
    if (!db)
        return false;

    const char* sql =
        "SELECT current_face, cube_state FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        face = sqlite3_column_int(stmt, 0);
        cubeState = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

// =========================
// RESET SESSION
// =========================
void Database::resetSession()
{
    if (!db) return;

    const char* sql = "DELETE FROM session WHERE id = 1;";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

// =========================
// CHECK SESSION
// =========================
bool Database::hasSession()
{
    if (!db) return false;

    const char* sql = "SELECT COUNT(*) FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}