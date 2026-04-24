#include "Validator.h"

// =========================
// FACE VALIDATION
// =========================
bool Validator::isValidFace(const std::array<Colour, 9>& face)
{
    for (auto c : face)
    {
        if (c == Colour::UNKNOWN)
            return false;

        if (c < Colour::WHITE || c > Colour::ORANGE)
            return false;
    }
    return true;
}

// =========================
// CUBE VALIDATION
// =========================
bool Validator::isValidCube(const Cube& cube)
{
    const auto& state = cube.getState();

    std::array<int, 6> count = { 0 };

    for (auto c : state)
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
        default: return false;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        if (count[i] != 9)
            return false;
    }

    return true;
}



// =========================
// VALIDATION
// =========================
//bool MainWindow::isValidFace(const std::array<Colour, 9>& face)
//{
//    for (auto c : face)
//    {
//        if (c == Colour::UNKNOWN)
//            return false;
//
//        // optional: ensure valid enum range
//        if (c < Colour::WHITE || c > Colour::ORANGE)
//            return false;
//    }
//
//    return true;
//}