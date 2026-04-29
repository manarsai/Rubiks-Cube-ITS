#pragma once

#include <vector>
#include <string>

#include "StageDefinitions.h"
#include "Cube.h"
#include "Colour.h"

class Solver
{
public:
    Solver() = default;
    ~Solver() = default;

    // =========================
    // IDA* STAGE SOLVER
    // =========================
    std::vector<std::string> solveToStage(
        const Cube& cube,
        Stage targetStage
    );

    // =========================
    // RECOVERY ENTRY POINT
    // =========================
    std::vector<std::string> recover(const Cube& cube);

    // =========================
    // HEURISTIC (used by IDA*)
    // =========================
    int heuristic(const Cube& cube, Stage stage) const;

private:
    // =========================
    // FUTURE EXTENSIONS (optional)
    // =========================
    // These are intentionally left out until needed for F2L/OLL optimizations.

    /*
    bool isEdgeCorrect(int edgeIndex, const Cube& cube) const;
    bool isEdgeOriented(int edgeIndex, const Cube& cube) const;
    bool isValidTransition(const Cube& cube, Stage targetStage) const;
    */
};