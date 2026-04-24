#include "Database.h"
#include <iostream>
#include <QCoreApplication>

// =========================
// SINGLETON
// =========================
Database& Database::getInstance()
{
    static Database instance;
    return instance;
}


// OPEN DB
bool Database::open()
{
    QString dbPath = QCoreApplication::applicationDirPath() + "/rubiksdb.db";

    std::cout << "DB PATH: " << dbPath.toStdString() << "\n";

    int rc = sqlite3_open(dbPath.toStdString().c_str(), &db);

    if (rc != SQLITE_OK)
    {
        std::cout << "DB OPEN FAILED\n";
        db = nullptr;
        return false;
    }

    std::cout << "DB OPEN SUCCESS\n";
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
// INIT TABLES
// =========================
void Database::initTables()
{
    if (!db) return;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS session ("
        "id INTEGER PRIMARY KEY,"
        "current_face INTEGER,"
        "cube_state TEXT);"

        "CREATE TABLE IF NOT EXISTS student ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE,"
        "stage INTEGER);"

        "CREATE TABLE IF NOT EXISTS stage_stats ("
        "name TEXT,"
        "stage INTEGER,"
        "success INTEGER,"
        "fail INTEGER,"
        "solver INTEGER,"
        "time REAL,"
        "PRIMARY KEY(name, stage));";

    char* err = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &err);

    if (err)
    {
        std::cout << "DB INIT ERROR: " << err << "\n";
        sqlite3_free(err);
    }
}

// =========================
// SESSION
// =========================
void Database::saveSession(int face, const std::string& cubeState)
{
    if (!db) return;

    const char* sql =
        "INSERT OR REPLACE INTO session (id, current_face, cube_state) "
        "VALUES (1, ?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, face);
    sqlite3_bind_text(stmt, 2, cubeState.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool Database::loadSession(int& face, std::string& cubeState)
{
    if (!db) return false;

    const char* sql =
        "SELECT current_face, cube_state FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

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

void Database::resetSession()
{
    if (!db) return;

    sqlite3_exec(db, "DELETE FROM session WHERE id = 1;", nullptr, nullptr, nullptr);
}

bool Database::hasSession()
{
    if (!db) return false;

    const char* sql = "SELECT COUNT(*) FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
        exists = sqlite3_column_int(stmt, 0) > 0;

    sqlite3_finalize(stmt);
    return exists;
}

// =========================
// STUDENT
// =========================
void Database::saveStudent(const std::string& name, int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO student(name, stage) VALUES(?, ?) "
        "ON CONFLICT(name) DO UPDATE SET stage=excluded.stage;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool Database::loadStudent(std::string& name, int& stage)
{
    if (!db) return false;

    const char* sql =
        "SELECT name, stage FROM student ORDER BY id DESC LIMIT 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        stage = sqlite3_column_int(stmt, 1);
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

// =========================
// STAGE STATS (SAFE UPSERT)
// =========================
void Database::updateSuccess(const std::string& name, int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(name, stage, success, fail, solver, time) "
        "VALUES(?, ?, 1, 0, 0, 0) "
        "ON CONFLICT(name, stage) DO UPDATE SET success = success + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateFailure(const std::string& name, int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(name, stage, success, fail, solver, time) "
        "VALUES(?, ?, 0, 1, 0, 0) "
        "ON CONFLICT(name, stage) DO UPDATE SET fail = fail + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateSolverUse(const std::string& name, int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(name, stage, success, fail, solver, time) "
        "VALUES(?, ?, 0, 0, 1, 0) "
        "ON CONFLICT(name, stage) DO UPDATE SET solver = solver + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateTime(const std::string& name, int stage, double seconds)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(name, stage, success, fail, solver, time) "
        "VALUES(?, ?, 0, 0, 0, ?) "
        "ON CONFLICT(name, stage) DO UPDATE SET time = time + ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);
    sqlite3_bind_double(stmt, 3, seconds);
    sqlite3_bind_double(stmt, 4, seconds);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::getStageStats(const std::string& name, int stage,
    int& success, int& fail, int& solver, double& time)
{
    if (!db) return;

    const char* sql =
        "SELECT success, fail, solver, time "
        "FROM stage_stats WHERE name=? AND stage=?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        success = sqlite3_column_int(stmt, 0);
        fail = sqlite3_column_int(stmt, 1);
        solver = sqlite3_column_int(stmt, 2);
        time = sqlite3_column_double(stmt, 3);
    }

    sqlite3_finalize(stmt);
}