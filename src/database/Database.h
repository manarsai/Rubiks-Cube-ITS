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

    void saveSession(int face, const std::string& cubeState);
    bool loadSession(int& face, std::string& cubeState);

    void resetSession();
    bool hasSession();

private:
    Database() = default;
    ~Database() = default;

    sqlite3* db = nullptr;
};