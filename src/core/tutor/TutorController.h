#pragma once

#include "../domain/Cube.h"
#include "../domain/StageDefinitions.h"
#include "../student/Student.h"
#include "Tutor.h"
#include "../../visual/ScanType.h"
#include "Guidance.h"

class TutorController
{
public:
    TutorController(Student& student);

    struct TutorResult
    {
        int stageValue;
        std::string message;
        std::string instructionText;
    };

    TutorResult CheckSubmission(const Cube& cube, ScanType type);

    // =====================================================
    // FIX: expose expected stage to UI / AppController
    // =====================================================
    Stage getExpectedStage() const;

private:
    Stage expectedStage;

    Student& student;

    Tutor engine;
};