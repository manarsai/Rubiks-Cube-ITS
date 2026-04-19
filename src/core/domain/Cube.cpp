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
    for (int i = 0; i < 9; i++)  state[i] = Colour::YELLOW; // U
    for (int i = 9; i < 18; i++) state[i] = Colour::BLUE;   // L
    for (int i = 18; i < 27; i++) state[i] = Colour::RED;   // F
    for (int i = 27; i < 36; i++) state[i] = Colour::GREEN; // R
    for (int i = 36; i < 45; i++) state[i] = Colour::ORANGE; // B
    for (int i = 45; i < 54; i++) state[i] = Colour::WHITE;  // D

}

// ===============================
// MOVE APPLICATION
// ===============================
void Cube::applyMove(const Move& m) {
    std::array<Colour, 54> newState;

    for (int i = 0; i < 54; i++) {
        newState[i] = state[m.perm[i]];
    }

    state = newState;
}

// ===============================
// GET / SET STATE
// ===============================
const std::array<Colour, 54>& Cube::getState() const {
    return state;
}

void Cube::setState(const std::array<Colour, 54>& newState) {
    state = newState;
}

// ===============================
// PRINT (DEBUG)
// ===============================
void Cube::print() const {
    for (int i = 0; i < 54; i++) {
        std::cout << colourToString(state[i]) << " ";

        if ((i + 1) % 3 == 0) std::cout << "\n";
        if ((i + 1) % 9 == 0) std::cout << "\n";
    }
}

// ===============================
// COLOR STRING
// ===============================
std::string Cube::colourToString(Colour c) {
    switch (c) {
    case Colour::WHITE:  return "W";
    case Colour::RED:    return "R";
    case Colour::BLUE:   return "B";
    case Colour::YELLOW: return "Y";
    case Colour::ORANGE: return "O";
    case Colour::GREEN:  return "G";
    default:            return "?";
    }
}

// =====================================================
// ?? SCANNING SUPPORT (NEW ADDITIONS)
// =====================================================


void Cube::setFace(Face face, const std::array<Colour, 9>& faceColors)
{
    int offset = static_cast<int>(face) * 9;

    for (int i = 0; i < 9; i++)
    {
        state[offset + i] = faceColors[i];
    }
}