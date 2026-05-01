#include "AppController.h"
#include "../src/core/domain/Validator.h"
#include "../src/core/domain/StageDefinitions.h"
#include "../src/core/tutor/TutorController.h"

#include <iostream>

AppController::AppController()
    : student("Guest"),
    tutorController(student),
    currentFace(0),
    scanComplete(false),
    solverMode(false)
{
    cube.reset();
}

// =====================================================
// MAIN SCAN PROCESS
// =====================================================
AppController::ScanResult AppController::processScan(
    const std::array<Colour, 9>& face)
{
    ScanResult result;

    // =====================================
    // BLOCK IF SCAN ALREADY COMPLETE
    // =====================================
    if (scanComplete)
    {
        result.success = false;
        result.message = "Scan complete. Please start a new scan.";
        return result;
    }

    // =====================================
    // SAFETY CHECK
    // =====================================
    if (currentFace < 0 || currentFace > 5)
    {
        result.success = false;
        result.message = "Invalid scan state. Resetting.";
        resetScan();
        return result;
    }

    // =====================================
    // CHECK FOR EMPTY SCAN
    // =====================================
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

    // =====================================
    // VALIDATE SCAN QUALITY
    // =====================================
    if (!Validator::isScanSafe(face))
    {
        result.success = false;
        result.message = "Scan too unclear.";
        return result;
    }

    // =====================================
    // STORE FACE
    // =====================================
    cube.setFace(currentFace, face);
    currentFace++;

    // =====================================
    // NOT FINISHED SCANNING
    // =====================================
    if (currentFace < 6)
    {
        result.success = true;
        result.finished = false;
        result.stageValue = currentFace;
        result.message = "Face recorded.";
        return result;
    }

    // =====================================
    // FINAL VALIDATION
    // =====================================
    if (!Validator::isCubeComplete(cube) || !Validator::isValidCube(cube))
    {
        result.success = false;
        result.message = "Invalid cube scan. Please rescan.";
        resetCube(); // ?? FULL RESET (important)
        return result;
    }

    // =====================================
    // TUTOR (SOURCE OF TRUTH)
    // =====================================
    auto tutorResult =
        tutorController.CheckSubmission(cube, ScanType::SUBMIT);

    Stage expectedStage = tutorController.getExpectedStage();

    std::cout << "Expected stage: " << (int)expectedStage << "\n";

    result.success = true;
    result.finished = true;
    result.stageValue = tutorResult.stageValue;
    result.message = tutorResult.message;
    result.guidance = tutorResult.instructionText;

    // =====================================
    // SOLVER (FIXED: USE COPY)
    // =====================================
    result.moves.clear();

    if (solverMode)
    {
        std::cout << "Running recovery solver...\n";

        Cube temp = cube;                 // ? COPY (CRITICAL FIX)
        result.moves = solver.recover(temp);
    }
    else
    {
        std::cout << "Solver skipped (normal mode)\n";
    }

    // =====================================
    // MARK COMPLETE
    // =====================================
    scanComplete = true;

    return result;
}

// =====================================================
// RESET SCAN ONLY (keeps cube)
// =====================================================
void AppController::resetScan()
{
    currentFace = 0;
    scanComplete = false;
}

// =====================================================
// RESET EVERYTHING
// =====================================================
void AppController::resetCube()
{
    cube.reset();          // must fully clear all faces
    currentFace = 0;
    scanComplete = false;
}

// =====================================================
// ACCESS
// =====================================================
Cube& AppController::getCube()
{
    return cube;
}

// =====================================================
// LOAD STATE
// =====================================================
void AppController::loadState(const std::string& state)
{
    cube.deserialize(state);
    resetScan(); // ? ensures fresh scan state
}

// =====================================================
// VALIDATION
// =====================================================
bool AppController::isCubeValid() const
{
    return cube.isValidCube();
}

// =====================================================
// SOLVER MODE
// =====================================================
void AppController::setSolverMode(bool mode)
{
    solverMode = mode;
}

bool AppController::isSolverMode() const
{
    return solverMode;
}

// =====================================================
// STUDENT ACCESS
// =====================================================
Student& AppController::getStudent()
{
    return student;
}

const Student& AppController::getStudent() const
{
    return student;
}

Stage AppController::getExpectedStage() const
{
    return tutorController.getExpectedStage();
}