#pragma once
#include <array>
#include "Cube.h"

class Validator
{
public:
    // =========================
    // FACE VALIDATION (scan input)
    // =========================
    static bool isValidFace(const std::array<Colour, 9>& face);

    // =========================
    // CUBE VALIDATION (full state)
    // =========================
    static bool isValidCube(const Cube& cube);
};