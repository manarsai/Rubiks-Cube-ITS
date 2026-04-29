#pragma once

#include <array>
#include <vector>
#include <string>

#include "../src/core/domain/types.h"
#include "../src/core/domain/Cube.h"
#include "../src/core/domain/Solver.h"
#include "../src/core/domain/Validator.h"

#include "../src/core/student/Student.h"
#include "../src/core/tutor/TutorController.h"
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
        bool success = false;
        bool finished = false;
        int stageValue = 0;

        std::vector<std::string> moves;  // solver output only

        std::string message;   // system / error text
        std::string guidance;  // user-facing instructions
    };

    // =====================================================
    // MAIN PIPELINE ENTRY POINT
    // =====================================================
    ScanResult processScan(const std::array<Colour, 9>& face);

    // =====================================================
    // STATE CONTROL
    // =====================================================
    void resetScan();
    void resetCube();
    void loadState(const std::string& state);

    // =====================================================
    // ACCESS
    // =====================================================
    Cube& getCube();
    bool isCubeValid() const;

    // =====================================================
    // SOLVER MODE
    // =====================================================
    void setSolverMode(bool mode);
    bool isSolverMode() const;

private:
    // =====================================================
    // CORE DOMAIN OBJECTS
    // =====================================================
    Cube cube;
    Solver solver;
    Student student;
    TutorController tutorController;

    // =====================================================
    // SCAN STATE
    // =====================================================
    int currentFace = 0;
    bool scanComplete = false;
    bool solverMode = false;
};