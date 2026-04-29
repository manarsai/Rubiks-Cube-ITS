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

TutorController::TutorResult TutorController::CheckSubmission(
    const Cube& cube,
    ScanType type)
{
    TutorResult result;

    // =====================================================
    // 0. HARD VALIDATION (BLOCK BAD SCANS)
    // =====================================================
    if (!Validator::isValidCube(cube))
    {
        std::cout << "Invalid cube scan - ignored\n";

        result.stageValue = static_cast<int>(expectedStage);
        result.message = "Invalid scan. Try again.";
        result.instructionText = "Make sure all faces are scanned clearly.";

        return result;
    }

    // =====================================================
    // 1. DETECT CURRENT STAGE
    // =====================================================
    Stage actualStage = StageDefinitions::detect(cube.getState());

    std::cout << "\n--- TUTOR DEBUG ---\n";
    std::cout << "Actual Stage   : " << (int)actualStage << "\n";
    std::cout << "Expected Stage : " << (int)expectedStage << "\n";

    // =====================================================
    // 2. INITIAL ENTRY (SCRAMBLED ? START LEARNING)
    // =====================================================
    if (expectedStage == Stage::SCRAMBLED)
    {
        expectedStage = (actualStage == Stage::SCRAMBLED)
            ? Stage::WHITE_CROSS   // beginner path
            : actualStage;         // jump to detected level
    }

    // =====================================================
    // 3. SUCCESS CHECK (ALLOW FORWARD PROGRESS)
    // =====================================================
    bool success = (actualStage >= expectedStage);

    std::cout << "Success        : " << success << "\n";

    // =====================================================
    // 4. UPDATE STUDENT MODEL
    // =====================================================
    if (success)
        student.recordSuccess(expectedStage);
    else
        student.recordFailure(expectedStage);

    // =====================================================
    // 5. DECIDE NEXT STAGE (ONLY ON SUCCESS)
    // =====================================================
    if (success)
    {
        expectedStage = engine.decideNextStage(
            expectedStage,
            actualStage,
            student
        );
    }

    std::cout << "New Expected   : " << (int)expectedStage << "\n";

    // =====================================================
    // 6. GENERATE GUIDANCE
    // =====================================================
    Guidance guidance;
    std::string instruction = guidance.generate(expectedStage, student);

    // =====================================================
    // 7. OUTPUT RESULT
    // =====================================================
    result.stageValue = static_cast<int>(expectedStage);
    result.message = success ? "Good progress" : "Try again";
    result.instructionText = instruction;

    return result;
}