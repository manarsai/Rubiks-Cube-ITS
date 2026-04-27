#pragma once
#include <string>
#include <sqlite3.h>

class Database
{
public:
    static Database& getInstance();

    bool open();
    void close();

    void initTables();

    // =========================
    // SESSION (SAVE/RESUME GAME)
    // =========================
// SESSION
    void saveSession(int face,
        const std::string& cubeState,
        int stage,
        const std::string& instruction,
        bool solverMode);

    bool loadSession(int& face,
        std::string& cubeState,
        int& stage,
        std::string& instruction,
        bool& solverMode);

    void resetSession();
    bool hasSession();

    // =========================
    // USER SETTINGS (PERSISTED)
    // =========================
    void setUserName(const std::string& name);
    const std::string& getUserName() const;
    bool loadUserName(std::string& name);

    // =========================
    // STAGE STATS (GLOBAL)
    // =========================
    void updateSuccess(int stage);
    void updateFailure(int stage);
    void updateSolverUse(int stage);
    void updateTime(int stage, double seconds);

    void getStageStats(int stage,
        int& success,
        int& fail,
        int& solver,
        double& time);

private:
    Database() = default;
    ~Database() = default;

    sqlite3* db = nullptr;

    // cached runtime user name
    std::string currentUserName;
};