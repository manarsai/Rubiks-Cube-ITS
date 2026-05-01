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

    struct ScanResult
    {
        bool success = false;
        bool finished = false;
        int stageValue = 0;

        std::vector<std::string> moves;

        std::string message;
        std::string guidance;
    };

    // =========================
    // STUDENT
    // =========================
    Student& getStudent();
    const Student& getStudent() const;

    // =========================
    // FIX 1: TUTOR ACCESS
    // =========================
    Stage getExpectedStage() const;

    // =========================
    // PIPELINE
    // =========================
    ScanResult processScan(const std::array<Colour, 9>& face);

    // =========================
    // STATE
    // =========================
    void resetScan();
    void resetCube();
    void loadState(const std::string& state);

    // =========================
    // CUBE
    // =========================
    Cube& getCube();
    const Cube& getCube() const;
    bool isCubeValid() const;

    // =========================
    // SOLVER
    // =========================
    void setSolverMode(bool mode);
    bool isSolverMode() const;

private:
    Cube cube;
    Solver solver;
    Student student;
    TutorController tutorController;

    int currentFace = 0;
    bool scanComplete = false;
    bool solverMode = false;
};