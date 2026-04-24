#include "AppController.h"
#include "../src/core/tutor/StageDefinitions.h"

// =========================
// INIT
// =========================
AppController::AppController()
    : student("Guest")
{
    cube.reset();
}

// =========================
// SCAN PIPELINE (FIXED)
// =========================
AppController::ScanResult AppController::processScan(
    const std::array<Colour, 9>& face)
{
    ScanResult result;

    // 1. Apply scanned face
    cube.setFace(currentFace, face);

    // 2. Validate cube after each scan
    if (!cube.isValidCube())
    {
        result.success = false;
        result.message = "Invalid cube state detected.";
        return result;
    }

    currentFace++;

    // 3. Not finished scanning yet
    if (currentFace < 6)
    {
        result.finished = false;
        result.stageValue = currentFace;
        return result;
    }

    // 4. Full cube scanned ? analyze
    Stage stage = StageDefinitions::detect(cube);
    result.stageValue = static_cast<int>(stage);

    auto moves = solver.solveWhiteCross(cube);

    result.finished = true;

    for (const auto& m : moves)
    {
        result.solutionText += m;
        result.solutionText += " ";
    }

    return result;
}

// =========================
// RESET SCAN STATE
// =========================
void AppController::resetScan()
{
    currentFace = 0;
}

// =========================
// CUBE CONTROL
// =========================
void AppController::resetCube()
{
    cube.reset();
    resetScan();
}

void AppController::setFace(int faceIndex, const std::array<Colour, 9>& face)
{
    cube.setFace(faceIndex, face);
}

Cube& AppController::getCube()
{
    return cube;
}

// =========================
// STATE
// =========================
void AppController::loadState(const std::string& state)
{
    cube.deserialize(state);
}

// =========================
// VALIDATION
// =========================
bool AppController::isCubeValid() const
{
    return cube.isValidCube();
}

// =========================
// ANALYSIS
// =========================
Stage AppController::detectStage()
{
    return StageDefinitions::detect(cube);
}

// =========================
// SOLVER
// =========================
std::vector<std::string> AppController::solveWhiteCross()
{
    return solver.solveWhiteCross(cube);
}