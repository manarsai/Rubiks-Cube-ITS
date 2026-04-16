#pragma once
#include <sqlite3.h>
#include <string>

class Database
{
public:
    static Database& getInstance();

    bool open();
    void close();

    bool hasSession();

    void initTables();

    void saveSession(int face, const std::string& cubeState);
    bool loadSession(int& face, std::string& cubeState);
    void resetSession();



private:
    Database() = default;
    sqlite3* db = nullptr;
};