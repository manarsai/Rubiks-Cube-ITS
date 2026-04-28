#pragma once

#include "../domain/Cube.h"
#include "../domain/StageDefinitions.h"

#include <vector>
#include <string>
#include <array>

class Solver
{
public:
    // =========================
    // GENERIC STAGE SOLVER
    // =========================
    std::vector<std::string> solveToStage(const Cube& cube, Stage targetStage);

    // =========================
    // RECOVERY SYSTEM
    // =========================
    std::vector<std::string> recover(const Cube& cube);

    // =========================
    // HEURISTIC (stage-aware)
    // =========================
    int heuristic(const std::array<Colour, 54>& state, Stage stage);

    // =========================
    // DEBUG
    // =========================
    void testMoveInverses();
};