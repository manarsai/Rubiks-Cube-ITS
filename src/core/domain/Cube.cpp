#include "Cube.h"
#include <sstream>

// =========================
// Constructor
// =========================
Cube::Cube()
{
    reset();
}

// =========================
// Reset (clean + explicit)
// =========================
void Cube::reset()
{
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(UP, r, c)] = Colour::YELLOW;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(LEFT, r, c)] = Colour::BLUE;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(FRONT, r, c)] = Colour::RED;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(RIGHT, r, c)] = Colour::GREEN;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(BACK, r, c)] = Colour::ORANGE;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(DOWN, r, c)] = Colour::WHITE;
}

// =========================
// State access
// =========================
const std::array<Colour, 54>& Cube::getState() const
{
    return state;
}

void Cube::setState(const std::array<Colour, 54>& newState)
{
    state = newState;
}

// =========================
// NEW: coordinate access
// =========================
Colour& Cube::at(int face, int row, int col)
{
    if (face < 0 || face > 5 ||
        row < 0 || row > 2 ||
        col < 0 || col > 2)
    {
        static Colour dummy = Colour::UNKNOWN;
        return dummy; // prevents crash
    }

    return state[idx(face, row, col)];
}
const Colour& Cube::at(int face, int row, int col) const
{
    return state[idx(face, row, col)];
}

// =========================
// Face access (scanner + UI)
// =========================
std::array<Colour, 9> Cube::getFace(int face) const
{
    std::array<Colour, 9> result{};
    int i = 0;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            result[i++] = state[idx(face, r, c)];

    return result;
}

void Cube::setFace(int face, const std::array<Colour, 9>& faceColors)
{
    int i = 0;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            state[idx(face, r, c)] = faceColors[i++];
}

// =========================
// Moves (unchanged for now)
// =========================
void Cube::applyMove(const Move& m)
{
    std::array<Colour, 54> newState;

    for (int i = 0; i < 54; i++)
        newState[i] = state[m.perm[i]];

    state = newState;
}

// =========================
// Solved check
// =========================
bool Cube::isSolved() const
{
    for (int f = 0; f < 6; f++)
    {
        Colour center = state[idx(f, 1, 1)];

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (state[idx(f, r, c)] != center)
                    return false;
    }

    return true;
}

// =========================
// Serialize
// =========================
std::string Cube::serialize() const
{
    std::ostringstream out;

    for (int i = 0; i < 54; i++)
    {
        out << static_cast<int>(state[i]);
        if (i < 53) out << ",";
    }

    return out.str();
}

// =========================
// Deserialize
// =========================
void Cube::deserialize(const std::string& str)
{
    std::istringstream ss(str);
    std::string val;

    int i = 0;

    while (std::getline(ss, val, ',') && i < 54)
    {
        state[i] = static_cast<Colour>(std::stoi(val));
        i++;
    }
}

// =========================
// Colour helper
// =========================
std::string Cube::colourToString(Colour c)
{
    switch (c)
    {
    case Colour::WHITE:  return "W";
    case Colour::RED:    return "R";
    case Colour::BLUE:   return "B";
    case Colour::YELLOW: return "Y";
    case Colour::GREEN:  return "G";
    case Colour::ORANGE: return "O";
    default:              return "?";
    }
}

bool Cube::isValidCube() const
{
    std::array<int, 6> count = { 0 };

    for (auto c : state)
    {
        if (c == Colour::UNKNOWN)
            return false;

        count[(int)c]++;
    }

    for (int i = 0; i < 6; i++)
    {
        if (count[i] != 9)
            return false;
    }

    return true;
}

