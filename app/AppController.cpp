#include "AppController.h"
#include "../src/core/domain/Validator.h"
#include "../src/core/domain/StageDefinitions.h"
#include "../src/core/tutor/TutorController.h"

#include <iostream>

// =========================
// INIT
// =========================
AppController::AppController()
    : student("Guest"),
    tutorController(student),
    currentFace(0),
    scanComplete(false),
    solverMode(false)
{
    cube.reset();
}

// =========================
// SCAN PROCESS
// =========================
AppController::ScanResult AppController::processScan(
    const std::array<Colour, 9>& face)
{
    ScanResult result;

    std::cout << "\n---- SCAN START ----\n";
    std::cout << "Current face index: " << currentFace << "\n";

    // =====================================================
    // 0. STATE SAFETY
    // =====================================================
    if (scanComplete)
    {
        result.success = false;
        result.message = "Scan complete. Reset to scan again.";
        return result;
    }

    if (currentFace < 0 || currentFace > 5)
    {
        result.success = false;
        result.message = "Invalid scan state. Reset required.";
        currentFace = 0;
        scanComplete = false;
        return result;
    }

    // =====================================================
    // 1. EMPTY SCAN CHECK
    // =====================================================
    bool hasData = false;
    for (Colour c : face)
    {
        if (c != Colour::UNKNOWN)
        {
            hasData = true;
            break;
        }
    }

    if (!hasData)
    {
        result.success = false;
        result.message = "Nothing was scanned.";
        return result;
    }

    // =====================================================
    // 2. QUALITY CHECK
    // =====================================================
    if (!Validator::isScanSafe(face))
    {
        result.success = false;
        result.message = "Scan too unclear.";
        return result;
    }

    // =====================================================
    // 3. STORE FACE
    // =====================================================
    cube.setFace(currentFace, face);
    std::cout << "Face stored: " << currentFace << "\n";

    currentFace++;

    // =====================================================
    // 4. PARTIAL SCAN
    // =====================================================
    if (currentFace < 6)
    {
        result.success = true;
        result.finished = false;
        result.stageValue = currentFace;
        result.message = "Face recorded.";
        return result;
    }

    std::cout << "All 6 faces scanned. Validating cube...\n";

    // =====================================================
    // 5. FULL VALIDATION
    // =====================================================
    if (!Validator::isCubeComplete(cube))
    {
        result.success = false;
        result.message = "Incomplete cube scan.";

        currentFace = 0;
        scanComplete = false;
        return result;
    }

    if (!Validator::isValidCube(cube))
    {
        result.success = false;
        result.message = "Invalid cube configuration.";

        currentFace = 0;
        scanComplete = false;
        return result;
    }

    std::cout << "Cube valid. Passing to Tutor...\n";

    // =====================================================
    // 6. TUTOR
    // =====================================================
    TutorController::TutorResult tutorResult =
        tutorController.CheckSubmission(cube, ScanType::SUBMIT);

    std::cout << "Tutor stage: " << tutorResult.stageValue << "\n";

    // =====================================================
    // 7. SOLVER (RECOVERY ONLY)
    // =====================================================
    result.moves.clear();

    if (solverMode && result.finished)
    {
        std::cout << "Running recovery solver...\n";
        result.moves = solver.recover(cube);
        std::cout << "Moves: " << result.moves.size() << "\n";
    }
    else
    {
        std::cout << "Solver skipped (normal mode)\n";
    }

    // =====================================================
    // 8. OUTPUT
    // =====================================================
    result.success = true;
    result.finished = true;
    result.stageValue = tutorResult.stageValue;

    result.message = tutorResult.message;
    result.guidance = tutorResult.instructionText;

    // =====================================================
    // 9. FINALIZE STATE (SAFE)
    // =====================================================
    if (result.success)
        scanComplete = true;

    std::cout << "---- SCAN END ----\n";

    return result;
}

// =========================
// RESET SCAN
// =========================
void AppController::resetScan()
{
    currentFace = 0;
    scanComplete = false;
}

// =========================
// RESET CUBE
// =========================
void AppController::resetCube()
{
    cube.reset();
    currentFace = 0;
    scanComplete = false;
}

// =========================
// ACCESS
// =========================
Cube& AppController::getCube()
{
    return cube;
}

// =========================
// LOAD STATE
// =========================
void AppController::loadState(const std::string& state)
{
    cube.deserialize(state);
    currentFace = 0;
    scanComplete = false;
}

// =========================
// VALIDATION
// =========================
bool AppController::isCubeValid() const
{
    return cube.isValidCube();
}

// =========================
// SOLVER MODE
// =========================
void AppController::setSolverMode(bool mode)
{
    solverMode = mode;
}

bool AppController::isSolverMode() const
{
    return solverMode;
}