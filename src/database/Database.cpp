#include "Database.h"
#include <QCoreApplication>
#include <iostream>
#include <stdexcept>
#include <QDir>
#include <QStandardPaths>

// =========================
// RAII DELETERS
// =========================
void Database::SqliteDeleter::operator()(sqlite3* db) const
{
    if (db) sqlite3_close(db);
}

void Database::SqliteDeleter::operator()(sqlite3_stmt* stmt) const
{
    if (stmt) sqlite3_finalize(stmt);
}

// =========================
// SINGLETON
// =========================
Database& Database::instance()
{
    static Database inst;
    return inst;
}

// =========================
// LOG
// =========================
void Database::log(const std::string& msg)
{
    std::cout << "[DB] " << msg << std::endl;
}

// =========================
// OPEN / CLOSE
// =========================

void Database::open()
{
    QString dbDir = "C:/finalyearproject/RUBIKSCUBE_ITS/src/data";

    // ensure folder exists
    QDir dir(dbDir);
    if (!dir.exists())
        dir.mkpath(".");

    QString path = dbDir + "/rubiksdb.db";

    sqlite3* raw = nullptr;

    if (sqlite3_open(path.toStdString().c_str(), &raw) != SQLITE_OK)
        throw std::runtime_error("Failed to open DB");

    db_.reset(raw);

    log("Opened database at: " + path.toStdString());
}

void Database::close()
{
    db_.reset();
    log("Closed database");
}

// =========================
// EXEC / PREPARE
// =========================
void Database::exec(const std::string& sql)
{
    if (!db_)
        throw std::runtime_error("Database not open");

    char* err = nullptr;

    if (sqlite3_exec(db_.get(), sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK)
    {
        std::string msg = err ? err : "SQL error";
        sqlite3_free(err);
        throw std::runtime_error(msg);
    }
}

Database::StmtPtr Database::prepare(const std::string& sql)
{
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cout << "SQL PREPARE FAILED: "
            << sqlite3_errmsg(db_.get())
            << std::endl;

        std::cout << "QUERY: " << sql << std::endl;

        throw std::runtime_error("Prepare failed");
    }

    return StmtPtr(stmt);
}

// =========================
// TRANSACTIONS
// =========================
void Database::begin() { exec("BEGIN TRANSACTION;"); }
void Database::commit() { exec("COMMIT;"); }
void Database::rollback() { exec("ROLLBACK;"); }

// =========================
// TABLES
// =========================
void Database::initTables()
{
    exec(R"(
        CREATE TABLE IF NOT EXISTS session (
            id INTEGER PRIMARY KEY,
            current_face INTEGER,
            cube_state TEXT,
            stage INTEGER,
            instruction TEXT,
            solver_mode INTEGER
        );

        CREATE TABLE IF NOT EXISTS app_settings (
            id INTEGER PRIMARY KEY,
            user_name TEXT
        );

        CREATE TABLE IF NOT EXISTS stage_stats (
            stage INTEGER PRIMARY KEY,
            success INTEGER,
            fail INTEGER,
            solver INTEGER,
            time REAL
        );
    )");

    log("Tables initialized");
}

// =========================
// USER
// =========================
void Database::setUserName(const std::string& name)
{
    auto stmt = prepare(
        "INSERT OR REPLACE INTO app_settings (id, user_name) VALUES (1, ?);"
    );

    sqlite3_bind_text(stmt.get(), 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt.get());

    if (rc != SQLITE_DONE)
    {
        std::cout << "SQLITE ERROR: " << sqlite3_errmsg(db_.get()) << std::endl;
        return;
    }
}

std::optional<std::string> Database::getUserName()
{
    auto stmt = prepare("SELECT user_name FROM app_settings WHERE id = 1;");

    if (sqlite3_step(stmt.get()) == SQLITE_ROW)
    {
        const unsigned char* txt = sqlite3_column_text(stmt.get(), 0);
        return txt ? reinterpret_cast<const char*>(txt) : "";
    }

    return std::nullopt;
}

// =========================
// SESSION SAVE (STRUCT)
// =========================
void Database::saveSession(const Session& s)
{
    begin();

    try
    {
        auto stmt = prepare(R"(
            INSERT INTO session (id, current_face, cube_state, stage, instruction, solver_mode)
            VALUES (1, ?, ?, ?, ?, ?)
            ON CONFLICT(id) DO UPDATE SET
                current_face=excluded.current_face,
                cube_state=excluded.cube_state,
                stage=excluded.stage,
                instruction=excluded.instruction,
                solver_mode=excluded.solver_mode;
        )");

        sqlite3_bind_int(stmt.get(), 1, s.face);
        sqlite3_bind_text(stmt.get(), 2, s.cubeState.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt.get(), 3, s.stage);
        sqlite3_bind_text(stmt.get(), 4, s.instruction.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt.get(), 5, s.solverMode);

        sqlite3_step(stmt.get());

        commit();
    }
    catch (...)
    {
        rollback();
        throw;
    }
}

// =========================
// SESSION SAVE (UI FRIENDLY)
// =========================
void Database::saveSession(int face,
    const std::string& cubeState,
    int stage,
    const std::string& instruction,
    bool solverMode)
{
    Session s;
    s.face = face;
    s.cubeState = cubeState;
    s.stage = stage;
    s.instruction = instruction;
    s.solverMode = solverMode;

    saveSession(s);
}

// =========================
// SESSION LOAD
// =========================
bool Database::loadSession(int& face,
    std::string& cubeState,
    int& stage,
    std::string& instruction,
    bool& solverMode)
{
    auto stmt = prepare(R"(
        SELECT current_face, cube_state, stage, instruction, solver_mode
        FROM session WHERE id = 1;
    )");

    if (sqlite3_step(stmt.get()) != SQLITE_ROW)
        return false;

    face = sqlite3_column_int(stmt.get(), 0);

    const unsigned char* state = sqlite3_column_text(stmt.get(), 1);
    cubeState = state ? reinterpret_cast<const char*>(state) : "";

    stage = sqlite3_column_int(stmt.get(), 2);

    const unsigned char* instr = sqlite3_column_text(stmt.get(), 3);
    instruction = instr ? reinterpret_cast<const char*>(instr) : "";

    solverMode = sqlite3_column_int(stmt.get(), 4) != 0;

    return true;
}

// =========================
// SESSION CHECK
// =========================
bool Database::hasSession()
{
    auto stmt = prepare("SELECT 1 FROM session WHERE id = 1 LIMIT 1;");
    return sqlite3_step(stmt.get()) == SQLITE_ROW;
}

// =========================
// RESET
// =========================
void Database::resetSession()
{
    exec("DELETE FROM session WHERE id = 1;");
}

