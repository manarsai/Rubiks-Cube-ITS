#pragma once

#include <array>
#include <string>

enum class Colour {
    UNKNOWN,
    WHITE,
    RED,
    BLUE,
    GREEN,
    YELLOW,
    ORANGE
};

enum class Face {
    Up,
    Down,
    Left,
    Right,
    Front,
    Back
};

struct Move;

class Cube {
private:
    std::array<Colour, 54> state;

public:
    Cube();

    void reset();
    void applyMove(const Move& m);

    const std::array<Colour, 54>& getState() const;
    void setState(const std::array<Colour, 54>& newState);

    // ? THIS is the ONLY scanning function you need
    void setFace(Face face, const std::array<Colour, 9>& faceColors);

    void print() const;

    static std::string colourToString(Colour c);
};