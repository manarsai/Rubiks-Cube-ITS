#include "Student.h"

// =========================
// CONSTRUCTOR
// =========================
Student::Student(const std::string& name)
    : name(name), currentStage(Stage::SCRAMBLED)
{
}

// =========================
// BASIC INFO
// =========================
void Student::setName(const std::string& n)
{
    name = n;
}

std::string Student::getName() const
{
    return name;
}

void Student::setCurrentStage(Stage stage)
{
    currentStage = stage;
}

Stage Student::getCurrentStage() const
{
    return currentStage;
}

// =========================
// EVENTS
// =========================
void Student::recordSuccess(Stage stage)
{
    stats[stage].successCount++;
}

void Student::recordFailure(Stage stage)
{
    stats[stage].failCount++;
}

void Student::recordSolverUse(Stage stage)
{
    stats[stage].solverUses++;
}

// =========================
// TIMER
// =========================
void Student::startTimer(Stage stage)
{
    startTime = std::chrono::steady_clock::now();
    timerRunning = true;
}

void Student::stopTimer(Stage stage)
{
    if (!timerRunning)
        return;

    auto end = std::chrono::steady_clock::now();

    double seconds =
        std::chrono::duration<double>(end - startTime).count();

    stats[stage].timeSpent += seconds;

    timerRunning = false;
}

// =========================
// DATA ACCESS
// =========================
StageStats Student::getStats(Stage stage) const
{
    auto it = stats.find(stage);

    if (it != stats.end())
        return it->second;

    return StageStats{};
}

double Student::getSuccessRate(Stage stage) const
{
    auto s = getStats(stage);

    int total = s.successCount + s.failCount;

    if (total == 0)
        return 0.0;

    return (double)s.successCount / total;
}

// =========================
// SUPPORT LEVEL (ITS CORE)
// =========================
int Student::getSupportLevel(Stage stage) const
{
    auto s = getStats(stage);

    double rate = getSuccessRate(stage);

    if (rate < 0.4 || s.failCount > 5 ||
        s.solverUses > 3 || s.timeSpent > 120)
    {
        return 2; // high guidance
    }

    if (rate < 0.7 || s.failCount > 2 || s.timeSpent > 60)
    {
        return 1; // medium guidance
    }

    return 0; // minimal guidance
}