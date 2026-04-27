#pragma once

#include <array>
#include <string>

#include "../src/core/domain/Cube.h"
#include "../src/core/domain/Solver.h"
#include "../src/core/domain/Validator.h"

#include "../src/core/student/Student.h"
#include "../src/core/tutor/TutorController.h"
#include "../src/core/tutor/Tutor.h"
#include "../src/visual/ScanType.h"

class AppController
{
public:
    AppController();

    // =====================================================
    // RESULT RETURNED TO UI
    // =====================================================
    struct ScanResult
    {
        bool success = true;
        bool finished = false;

        std::string message;

        int stageValue = 0;
        std::string solutionText;

    };

    // =====================================================
    // MAIN PIPELINE ENTRY POINT
    // =====================================================
    ScanResult processScan(const std::array<Colour, 9>& face);

    void resetScan();
    void resetCube();

    Cube& getCube();
    bool scanComplete = false;

    bool isCubeValid() const;
    void loadState(const std::string& state);

private:
    Cube cube;

    // (Optional: remove later if Tutor fully owns solving)
    Solver solver;

    Student student;

    TutorController tutorController;

    int currentFace = 0;
};