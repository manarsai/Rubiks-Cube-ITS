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
    // SESSION
    // =========================
    void saveSession(int face, const std::string& cubeState);
    bool loadSession(int& face, std::string& cubeState);
    void resetSession();

    // =========================
    // STUDENT
    // =========================
    void saveStudent(const std::string& name, int stage);
    bool loadStudent(std::string& name, int& stage);

    // =========================
    // STAGE STATS
    // =========================
    void updateSuccess(const std::string& name, int stage);
    void updateFailure(const std::string& name, int stage);
    void updateSolverUse(const std::string& name, int stage);
    void updateTime(const std::string& name, int stage, double seconds);

    void getStageStats(const std::string& name, int stage,
        int& success, int& fail, int& solver, double& time);

    bool hasSession();

private:
    Database() = default;
    ~Database() = default;

    sqlite3* db = nullptr;
};