#include "Validator.h"

// =====================================================
// FACE VALIDATION (loose for camera input)
// =====================================================
bool Validator::isValidFace(const std::array<Colour, 9>& face)
{
    int validCount = 0;

    for (const auto& c : face)
    {
        if (c != Colour::UNKNOWN)
            validCount++;
    }

    return validCount >= 6; // tolerate noise
}

// =====================================================
// STRICT FACE (optional debug)
// =====================================================
bool Validator::isCompleteFace(const std::array<Colour, 9>& face)
{
    for (const auto& c : face)
    {
        if (c == Colour::UNKNOWN)
            return false;
    }
    return true;
}

// =====================================================
// SCAN SAFETY
// =====================================================
bool Validator::isScanSafe(const std::array<Colour, 9>& face)
{
    int detected = 0;

    for (const auto& c : face)
    {
        if (c != Colour::UNKNOWN)
            detected++;
    }

    return detected >= 6;
}

// =====================================================
// CUBE COMPLETENESS
// =====================================================
bool Validator::isCubeComplete(const Cube& cube)
{
    const auto& state = cube.getState();

    for (const auto& c : state)
    {
        if (c == Colour::UNKNOWN)
            return false;
    }

    return true;
}

// =====================================================
// FINAL VALIDATION
// =====================================================
bool Validator::isValidCube(const Cube& cube)
{
    const auto& state = cube.getState();

    if (state.size() != 54)
        return false;

    std::array<int, 6> count{};
    count.fill(0);

    for (const auto& c : state)
    {
        if (c == Colour::UNKNOWN)
            return false;

        switch (c)
        {
        case Colour::WHITE:  count[0]++; break;
        case Colour::RED:    count[1]++; break;
        case Colour::BLUE:   count[2]++; break;
        case Colour::YELLOW: count[3]++; break;
        case Colour::GREEN:  count[4]++; break;
        case Colour::ORANGE: count[5]++; break;

        default:
            return false;
        }
    }

    for (int i = 0; i < 6; i++)
        if (count[i] != 9)
            return false;

    return true;
}