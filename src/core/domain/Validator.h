#pragma once

#include <array>
#include "Cube.h"

class Validator
{
public:
    // ===== scanning stage =====
    static bool isValidFace(const std::array<Colour, 9>& face);
    static bool isScanSafe(const std::array<Colour, 9>& face);

    // ===== cube stage =====
    static bool isCubeComplete(const Cube& cube);
    static bool isValidCube(const Cube& cube);

private:
    static bool isCompleteFace(const std::array<Colour, 9>& face);
};