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

// =========================
// OPEN DB
// =========================
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
// INIT TABLES (UPDATED)
// =========================
void Database::initTables()
{
    if (!db) return;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS session ("
        "id INTEGER PRIMARY KEY,"
        "current_face INTEGER,"
        "cube_state TEXT,"
        "stage INTEGER,"
        "instruction TEXT,"
        "solver_mode INTEGER);"

        "CREATE TABLE IF NOT EXISTS app_settings ("
        "id INTEGER PRIMARY KEY,"
        "user_name TEXT);"

        "CREATE TABLE IF NOT EXISTS stage_stats ("
        "stage INTEGER PRIMARY KEY,"
        "success INTEGER,"
        "fail INTEGER,"
        "solver INTEGER,"
        "time REAL);";

    char* err = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &err);

    if (err)
    {
        std::cout << "DB INIT ERROR: " << err << "\n";
        sqlite3_free(err);
    }
}

// =========================
// USER NAME
// =========================
void Database::setUserName(const std::string& name)
{
    currentUserName = name;

    if (!db) return;

    const char* sql =
        "INSERT OR REPLACE INTO app_settings (id, user_name) "
        "VALUES (1, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

const std::string& Database::getUserName() const
{
    return currentUserName;
}

// =========================
// SESSION SAVE (UPDATED)
// =========================
void Database::saveSession(int face,
    const std::string& cubeState,
    int stage,
    const std::string& instruction,
    bool solverMode)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO session (id, current_face, cube_state, stage, instruction, solver_mode) "
        "VALUES (1, ?, ?, ?, ?, ?) "
        "ON CONFLICT(id) DO UPDATE SET "
        "current_face=excluded.current_face, "
        "cube_state=excluded.cube_state, "
        "stage=excluded.stage, "
        "instruction=excluded.instruction, "
        "solver_mode=excluded.solver_mode;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return;

    sqlite3_bind_int(stmt, 1, face);
    sqlite3_bind_text(stmt, 2, cubeState.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, stage);
    sqlite3_bind_text(stmt, 4, instruction.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, solverMode ? 1 : 0);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    std::cout << "SAVE SESSION -> face=" << face
        << " stage=" << stage
        << " solver=" << solverMode << std::endl;
}

// =========================
// SESSION LOAD (UPDATED)
// =========================
bool Database::loadSession(int& face,
    std::string& cubeState,
    int& stage,
    std::string& instruction,
    bool& solverMode)
{
    if (!db) return false;

    const char* sql =
        "SELECT current_face, cube_state, stage, instruction, solver_mode "
        "FROM session WHERE id = 1;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    bool ok = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        face = sqlite3_column_int(stmt, 0);

        const unsigned char* state = sqlite3_column_text(stmt, 1);
        cubeState = state ? reinterpret_cast<const char*>(state) : "";

        stage = sqlite3_column_int(stmt, 2);

        const unsigned char* instr = sqlite3_column_text(stmt, 3);
        instruction = instr ? reinterpret_cast<const char*>(instr) : "";

        solverMode = sqlite3_column_int(stmt, 4) != 0;

        ok = true;
    }

    sqlite3_finalize(stmt);
    return ok;
}

bool Database::loadUserName(std::string& name)
{
    if (!db) return false;

    const char* sql =
        "SELECT user_name FROM app_settings WHERE id = 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
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

    sqlite3_exec(db, "DELETE FROM session WHERE id = 1;", nullptr, nullptr, nullptr);
}

// =========================
// CHECK SESSION
// =========================
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
// STAGE STATS (UNCHANGED)
// =========================
void Database::updateSuccess(int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(stage, success, fail, solver, time) "
        "VALUES(?, 1, 0, 0, 0) "
        "ON CONFLICT(stage) DO UPDATE SET success = success + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateFailure(int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(stage, success, fail, solver, time) "
        "VALUES(?, 0, 1, 0, 0) "
        "ON CONFLICT(stage) DO UPDATE SET fail = fail + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateSolverUse(int stage)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(stage, success, fail, solver, time) "
        "VALUES(?, 0, 0, 1, 0) "
        "ON CONFLICT(stage) DO UPDATE SET solver = solver + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, stage);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::updateTime(int stage, double seconds)
{
    if (!db) return;

    const char* sql =
        "INSERT INTO stage_stats(stage, success, fail, solver, time) "
        "VALUES(?, 0, 0, 0, ?) "
        "ON CONFLICT(stage) DO UPDATE SET time = time + ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, stage);
    sqlite3_bind_double(stmt, 2, seconds);
    sqlite3_bind_double(stmt, 3, seconds);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::getStageStats(int stage,
    int& success, int& fail, int& solver, double& time)
{
    if (!db) return;

    const char* sql =
        "SELECT success, fail, solver, time "
        "FROM stage_stats WHERE stage=?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, stage);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        success = sqlite3_column_int(stmt, 0);
        fail = sqlite3_column_int(stmt, 1);
        solver = sqlite3_column_int(stmt, 2);
        time = sqlite3_column_double(stmt, 3);
    }

    sqlite3_finalize(stmt);
}