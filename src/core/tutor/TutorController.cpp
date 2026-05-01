#include "TutorController.h"
#include "../../visual/ScanType.h"
#include "../domain/StageDefinitions.h"
#include "../domain/Validator.h"
#include "Guidance.h"
#include <iostream>

TutorController::TutorController(Student& student)
    : student(student),
    expectedStage(Stage::SCRAMBLED)
{
}

// =====================================================
// GET EXPECTED STAGE (FIX: missing in your system)
// =====================================================
Stage TutorController::getExpectedStage() const
{
    return expectedStage;
}

// =====================================================
// MAIN LOGIC
// =====================================================
TutorController::TutorResult TutorController::CheckSubmission(
    const Cube& cube,
    ScanType type)
{
    TutorResult result;

    // =====================================================
    // 0. VALIDATION
    // =====================================================
    if (!Validator::isValidCube(cube))
    {
        result.stageValue = static_cast<int>(expectedStage);
        result.message = "Invalid scan";
        result.instructionText = "Try scanning again clearly.";
        return result;
    }

    // =====================================================
    // 1. DETECT ACTUAL STAGE (FOR LOGGING ONLY)
    // =====================================================
    Stage actualStage = StageDefinitions::detect(cube.getState());

    std::cout << "\n=== TUTOR DEBUG ===\n";
    std::cout << "Actual Stage   : " << (int)actualStage << "\n";
    std::cout << "Expected Stage : " << (int)expectedStage << "\n";

    // =====================================================
    // 2. INITIAL STATE SETUP
    // =====================================================
    if (expectedStage == Stage::SCRAMBLED)
    {
        expectedStage =
            (actualStage == Stage::SCRAMBLED)
            ? Stage::WHITE_CROSS
            : actualStage;
    }

    // =====================================================
    // 3. SUCCESS LOGIC
    // =====================================================
    bool success = StageDefinitions::validateStage(expectedStage, cube.getState());

    std::cout << "EXPECTED: " << (int)expectedStage << "\n";
    std::cout << "ACTUAL: " << (int)actualStage << "\n";
    std::cout << "SUCCESS: " << success << "\n";

    // =====================================================
    // 4. UPDATE STUDENT MODEL
    // =====================================================
    if (success)
        student.recordSuccess(expectedStage);
    else
        student.recordFailure(expectedStage);

    // =====================================================
    // 5. ADVANCE ONLY ON SUCCESS
    // =====================================================
    if (success)
    {
        expectedStage = engine.decideNextStage(expectedStage, actualStage, student);
    }

    std::cout << "Next Expected: " << (int)expectedStage << "\n";

    // =====================================================
    // 6. GUIDANCE (SOURCE OF TRUTH)
    // =====================================================
    std::string instruction =
        Guidance::generate(expectedStage, student);

    std::cout << "GUIDANCE STAGE: " << (int)expectedStage << "\n";

    // =====================================================
    // 7. RESULT
    // =====================================================
    result.stageValue = static_cast<int>(expectedStage);
    result.message = success ? "Good progress" : "";
    result.instructionText = instruction;

    return result;
}