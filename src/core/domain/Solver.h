#pragma once
#include "../domain/Cube.h"
#include <vector>
#include <string>

class Solver {
public:
    std::vector<std::string> solveWhiteCross(const Cube& cube);
    int heuristic(const std::array<Colour, 54>& s);
};