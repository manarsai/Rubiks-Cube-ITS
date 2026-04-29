#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

#include "Cube.h"
#include "Solver.h"
#include "StageDefinitions.h"
#include "Moves.h"

// =========================
// SETTINGS
// =========================
static const int INF = std::numeric_limits<int>::max();
static const int MAX_DEPTH = 20;
static const int NODE_LIMIT = 200000;

static int nodeCounter = 0;

// =========================
// INVERSE PRUNING
// =========================
static bool isInverse(const std::string& a, const std::string& b)
{
    return (a == "U" && b == "U'") || (a == "U'" && b == "U") ||
        (a == "D" && b == "D'") || (a == "D'" && b == "D") ||
        (a == "R" && b == "R'") || (a == "R'" && b == "R") ||
        (a == "L" && b == "L'") || (a == "L'" && b == "L") ||
        (a == "F" && b == "F'") || (a == "F'" && b == "F") ||
        (a == "B" && b == "B'") || (a == "B'" && b == "B");
}

// =========================
// HEURISTIC (WHITE CROSS ONLY)
// WHITE = DOWN FACE
// =========================
int Solver::heuristic(const Cube& cube, Stage stage) const
{
    if (stage != Stage::WHITE_CROSS)
        return 0;

    int misplaced = 0;

    struct Edge {
        int d_r, d_c;
        int f, f_r, f_c;
        Colour expected;
    };

    std::vector<Edge> edges = {
        {0, 1, FRONT, 2, 1, Colour::RED},
        {2, 1, BACK,  2, 1, Colour::ORANGE},
        {1, 0, LEFT,  2, 1, Colour::BLUE},
        {1, 2, RIGHT, 2, 1, Colour::GREEN}
    };

    for (auto& e : edges)
    {
        bool whiteOK = cube.at(DOWN, e.d_r, e.d_c) == Colour::WHITE;
        bool sideOK = cube.at(e.f, e.f_r, e.f_c) == e.expected;

        if (!whiteOK)
            misplaced++;

        if (!sideOK)
            misplaced++;
    }

    return misplaced;
}

// =========================
// DFS (IDA* CORE)
// =========================
static bool dfs(
    Cube& cube,
    Stage target,
    int g,
    int threshold,
    int& nextThreshold,
    std::vector<std::string>& path,
    const std::string& prevMove,
    Solver* solver)
{
    nodeCounter++;

    if (nodeCounter > NODE_LIMIT)
        return false;

    // GOAL CHECK
    if (StageDefinitions::validateStage(target, cube.getState()))
        return true;

    int h = solver->heuristic(cube, target);
    int f = g + h;

    if (f > threshold)
    {
        nextThreshold = std::min(nextThreshold, f);
        return false;
    }

    if (g >= MAX_DEPTH)
        return false;

    // ?? NOW USING GLOBAL MOVES TABLE (NO LOCAL DUPLICATION)
    for (auto& [name, move] : MOVES)
    {
        if (!prevMove.empty() && isInverse(prevMove, name))
            continue;

        Cube next = cube;
        next.applyMove(move);

        path.push_back(name);

        if (dfs(next, target, g + 1, threshold, nextThreshold, path, name, solver))
            return true;

        path.pop_back();
    }

    return false;
}

// =========================
// IDA* SOLVER
// =========================
std::vector<std::string> Solver::solveToStage(const Cube& startCube, Stage targetStage)
{
    nodeCounter = 0;

    int threshold = heuristic(startCube, targetStage);

    while (true)
    {
        int nextThreshold = INF;
        std::vector<std::string> path;

        Cube cube = startCube;

        bool found = dfs(
            cube,
            targetStage,
            0,
            threshold,
            nextThreshold,
            path,
            "",
            this
        );

        if (found)
            return path;

        if (nextThreshold == INF)
            return {};

        threshold = nextThreshold;
    }
}

// =========================
// RECOVERY LOGIC
// =========================
std::vector<std::string> Solver::recover(const Cube& cube)
{
    Stage current = StageDefinitions::detect(cube.getState());

    Stage target = Stage::WHITE_CROSS;

    switch (current)
    {
    case Stage::WHITE_CROSS: target = Stage::WHITE_CROSS; break;
    case Stage::F2L:         target = Stage::WHITE_CROSS; break;
    case Stage::OLL:         target = Stage::F2L; break;
    case Stage::PLL:         target = Stage::OLL; break;
    case Stage::COMPLETE:    target = Stage::PLL; break;
    default:                 target = Stage::WHITE_CROSS; break;
    }

    return solveToStage(cube, target);
}