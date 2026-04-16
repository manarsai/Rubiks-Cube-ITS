#include "Database.h"
#include <iostream>

Database& Database::getInstance()
{
    static Database instance;
    return instance;
}

bool Database::open()
{
    int rc = sqlite3_open("C:/finalyearproject/RUBIKSCUBE_ITS/src/data/rubiksdb.db", &db);

    if (rc)
    {
        std::cout << "DB open failed\n";
        return false;
    }

    return true;
}

void Database::close()
{
    if (db)
        sqlite3_close(db);
}

void Database::initTables()
{
    const char* sql =
        "CREATE TABLE IF NOT EXISTS session ("
        "id INTEGER PRIMARY KEY,"
        "current_face INTEGER,"
        "cube_state TEXT);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

void Database::saveSession(int face, const std::string& cubeState)
{
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
    const char* sql = "SELECT current_face, cube_state FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        face = sqlite3_column_int(stmt, 0);
        cubeState = (const char*)sqlite3_column_text(stmt, 1);
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

void Database::resetSession()
{
    const char* sql = "DELETE FROM session WHERE id = 1;";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

bool Database::hasSession()
{
    const char* sql = "SELECT COUNT(*) FROM session WHERE id = 1;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}