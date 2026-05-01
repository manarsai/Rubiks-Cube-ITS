#pragma once
#include <string>
#include <map>
#include <chrono>
#include "../domain/Stages.h"
#include "StageStats.h"


class Student
{
public:
    Student(const std::string& name = "Guest");

    // =========================
    // BASIC INFO
    // =========================
    void setName(const std::string& name);
    std::string getName() const;

    void setCurrentStage(Stage stage);
    Stage getCurrentStage() const;

    // =========================
    // EVENTS
    // =========================
    void recordSuccess(Stage stage);
    void recordFailure(Stage stage);
    void recordSolverUse(Stage stage);

    void startTimer(Stage stage);
    void stopTimer(Stage stage);

    // =========================
    // DATA
    // =========================
    StageStats getStats(Stage stage) const;
    double getSuccessRate(Stage stage) const;

    // =========================
    // ADAPTIVE SUPPORT
    // =========================
    int getSupportLevel(Stage stage) const;

private:
    std::string name;
    Stage currentStage;

    std::map<Stage, StageStats> stats;

    std::map<Stage, std::chrono::steady_clock::time_point> startTimes;
    std::map<Stage, bool> timerRunning;
};