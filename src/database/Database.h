#pragma once

#include <sqlite3.h>
#include <memory>
#include <string>
#include <optional>

#include "../core/student/StageStats.h"

// =====================================================
// SESSION DATA STRUCT
// NOTE: stage = TutorController expectedStage snapshot
// =====================================================
struct Session
{
    int face = 0;
    std::string cubeState;
    int stage = 0;          // IMPORTANT: represents expectedStage
    bool solverMode = false;
};

// =====================================================
// DATABASE CLASS
// =====================================================
class Database
{
public:
    static Database& instance();

    void open();
    void close();

    void initTables();

    // =====================================================
    // USER
    // =====================================================
    void setUserName(const std::string& name);
    std::optional<std::string> getUserName();

    // =====================================================
    // SESSION (CLEAN API)
    // =====================================================
    void saveSession(const Session& s);

    void saveSession(int face,
        const std::string& cubeState,
        int stage,
        bool solverMode);

    bool loadSession(int& face,
        std::string& cubeState,
        int& stage,
        bool& solverMode);

    bool hasSession();
    void resetSession();

    // =====================================================
    // STATS (OPTIONAL SYSTEM)
    // =====================================================
    void updateSuccess(int stage);
    void updateFailure(int stage);
    void updateSolverUse(int stage);
    void updateTime(int stage, double seconds);

    std::optional<StageStats> getStageStats(int stage);

    // =====================================================
    // DEBUG / SAFETY (ADDED)
    // =====================================================
    bool isOpen() const;

private:
    Database() = default;

    // =====================================================
    // CORE SQL HELPERS
    // =====================================================
    void exec(const std::string& sql);

    struct SqliteDeleter
    {
        void operator()(sqlite3* db) const;
        void operator()(sqlite3_stmt* stmt) const;
    };

    using DbPtr = std::unique_ptr<sqlite3, SqliteDeleter>;
    using StmtPtr = std::unique_ptr<sqlite3_stmt, SqliteDeleter>;

    StmtPtr prepare(const std::string& sql);

    void begin();
    void commit();
    void rollback();

    void log(const std::string& msg);

private:
    DbPtr db_;
};