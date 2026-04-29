#pragma once
#include <array>
#include <string>
#include "Moves.h"
#include "Colour.h"
#include "FaceDirections.h"

// =========================
// Cube
// =========================
class Cube
{
private:
    std::array<Colour, 54> state;

    // =========================
    // Core mapping function
    // =========================
    static constexpr int idx(int face, int row, int col)
    {
        return face * 9 + row * 3 + col;
    }

public:
    Cube();
    void reset();

    // =========================
    // State access
    // =========================
    const std::array<Colour, 54>& getState() const;
    void setState(const std::array<Colour, 54>& newState);

    // =========================
    // Face access (UI + scanner)
    // =========================
    std::array<Colour, 9> getFace(int face) const;
    void setFace(int face, const std::array<Colour, 9>& faceColors);

    // Optional cleaner access (VERY useful later)
    Colour& at(int face, int row, int col);
    const Colour& at(int face, int row, int col) const;

    // =========================
    // Moves
    // =========================
  

    void applyMove(const Move& m);

    // =========================
    // Utility
    // =========================
    bool isSolved() const;
    std::string serialize() const;
    void deserialize(const std::string& str);


    bool isValidCube() const;

    static std::string colourToString(Colour c);
};