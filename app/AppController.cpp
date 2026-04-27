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
    scanComplete(false)
{
    cube.reset();
}


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
        std::cout << "Scan session already complete. Please reset.\n";

        result.success = false;
        result.message = "Scan complete. Reset to scan again.";
        return result;
    }


    if (currentFace >= 6)
    {
        std::cout << "ERROR: Scan already complete.\n";
        result.success = false;
        result.message = "Scan already complete. Reset required.";
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
        std::cout << "ERROR: Empty scan.\n";
        result.success = false;
        result.message = "Nothing was scanned.";
        return result;
    }

    // =====================================================
    // 2. QUALITY CHECK
    // =====================================================
    if (!Validator::isScanSafe(face))
    {
        std::cout << "ERROR: Scan quality failed.\n";
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
    // 4. NOT COMPLETE YET
    // =====================================================
    if (currentFace < 6)
    {
        std::cout << "Scan incomplete. Waiting for more faces.\n";

        result.success = true;
        result.finished = false;
        result.stageValue = currentFace;
        result.message = "Face recorded.";
        return result;
    }

    std::cout << "All 6 faces scanned. Validating cube...\n";

    // =====================================================
    // 5. FULL CUBE VALIDATION
    // =====================================================
    if (!Validator::isCubeComplete(cube))
    {
        std::cout << "ERROR: Cube incomplete.\n";
        result.success = false;
        result.message = "Incomplete cube scan.";
        currentFace = 0;
        return result;
    }

    if (!Validator::isValidCube(cube))
    {
        std::cout << "ERROR: Invalid cube configuration.\n";

        result.success = false;
        result.message = "Invalid cube configuration.";

        currentFace = 0;        // ?? IMPORTANT FIX
        scanComplete = false;   // reset flow

        return result;
    }

    std::cout << "Cube valid. Passing to Tutor...\n";

    // =====================================================
    // 6. DOMAIN CHECK
    // =====================================================
    Stage detectedStage = StageDefinitions::detect(cube);
    std::cout << "Detected Stage (Domain): " << static_cast<int>(detectedStage) << "\n";

    // =====================================================
    // 7. HAND OFF TO TUTOR
    // =====================================================
    TutorController::TutorResult tutorResult =
        tutorController.CheckSubmission(cube, ScanType::SUBMIT);

    std::cout << "Tutor returned stage: " << tutorResult.stageValue << "\n";
    std::cout << "Tutor message: " << tutorResult.message << "\n";
    std::cout << "Tutor instruction: " << tutorResult.instructionText << "\n";

    // =====================================================
    // 8. COPY RESULT
    // =====================================================
    result.finished = true;
    result.success = true;

    result.stageValue = tutorResult.stageValue;
    result.message = tutorResult.message;
    result.moves = solver.solveWhiteCross(cube);

    // =====================================================
    // 9. RESET
    // =====================================================
    scanComplete = true;

    std::cout << "---- SCAN END ----\n";

    return result;
}

// =========================
// RESET SCAN STATE
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
}

// =========================
// VALIDATION
// =========================
bool AppController::isCubeValid() const
{
    return cube.isValidCube();
}