#include "Cube.h"
#include "Moves.h"
#include <iostream>
#include <stdexcept>
#include "../../vision/vision.h"

/*
Cube indexing:

0–8   = Up (Yellow)
9–17  = Left (Blue)
18–26 = Front (Red)
27–35 = Right (Green)
36–44 = Back (Orange)
45–53 = Down (White)
*/

Cube::Cube() {
    reset();
}

// ===============================
// RESET
// ===============================
void Cube::reset() {
    for (int i = 0; i < 9; i++)  state[i] = Color::YELLOW; // U
    for (int i = 9; i < 18; i++) state[i] = Color::BLUE;   // L
    for (int i = 18; i < 27; i++) state[i] = Color::RED;   // F
    for (int i = 27; i < 36; i++) state[i] = Color::GREEN; // R
    for (int i = 36; i < 45; i++) state[i] = Color::ORANGE; // B
    for (int i = 45; i < 54; i++) state[i] = Color::WHITE;  // D

}

// ===============================
// MOVE APPLICATION
// ===============================
void Cube::applyMove(const Move& m) {
    std::array<Color, 54> newState;

    for (int i = 0; i < 54; i++) {
        newState[i] = state[m.perm[i]];
    }

    state = newState;
}

// ===============================
// GET / SET STATE
// ===============================
const std::array<Color, 54>& Cube::getState() const {
    return state;
}

void Cube::setState(const std::array<Color, 54>& newState) {
    state = newState;
}

// ===============================
// PRINT (DEBUG)
// ===============================
void Cube::print() const {
    for (int i = 0; i < 54; i++) {
        std::cout << colorToString(state[i]) << " ";

        if ((i + 1) % 3 == 0) std::cout << "\n";
        if ((i + 1) % 9 == 0) std::cout << "\n";
    }
}

// ===============================
// COLOR STRING
// ===============================
std::string Cube::colorToString(Color c) {
    switch (c) {
    case Color::WHITE:  return "W";
    case Color::RED:    return "R";
    case Color::BLUE:   return "B";
    case Color::YELLOW: return "Y";
    case Color::ORANGE: return "O";
    case Color::GREEN:  return "G";
    default:            return "?";
    }
}

// =====================================================
// ?? SCANNING SUPPORT (NEW ADDITIONS)
// =====================================================


void Cube::setFace(Face face, const std::array<Color, 9>& faceColors)
{
    int offset = static_cast<int>(face) * 9;

    for (int i = 0; i < 9; i++)
    {
        state[offset + i] = faceColors[i];
    }
}