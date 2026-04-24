#pragma once

#include <array>
#include <vector>
#include <string>

#include "../src/core/domain/Cube.h"
#include "../src/core/domain/Solver.h"
#include "../src/core/student/Student.h"
#include "../src/core/tutor/Stages.h"

class AppController
{
public:
    AppController();

    // =========================
    // SCAN PIPELINE (NEW)
    // =========================
    struct ScanResult
    {
        bool success = true;
        bool finished = false;

        std::string message;

        int stageValue = 0;
        std::string solutionText;
    };

    ScanResult processScan(const std::array<Colour, 9>& face);

    void resetScan();

    // =========================
    // CORE CUBE CONTROL
    // =========================
    void resetCube();
    void setFace(int faceIndex, const std::array<Colour, 9>& face);

    Cube& getCube();

    // =========================
    // LOGIC
    // =========================
    bool isCubeValid() const;
    void loadState(const std::string& state);

    Stage detectStage();
    std::vector<std::string> solveWhiteCross();

private:
    Cube cube;
    Solver solver;
    Student student;

    // scan state
    int currentFace = 0;
};