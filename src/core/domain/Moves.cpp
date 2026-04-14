#include "Moves.h"
#include <array>
#include <vector>

// -------------------- Identity --------------------
static std::array<int, 54> makeIdentity() {
    std::array<int, 54> p{};
    for (int i = 0; i < 54; i++) p[i] = i;
    return p;
}

// helper: apply cycle (i0 i1 i2 i3) means i0->i1->i2->i3->i0
static void addCycle(std::array<int, 54>& p, std::vector<int> c) {
    for (int i = 0; i < c.size(); i++) {
        p[c[i]] = c[(i + 1) % c.size()];
    }
}


/*

Each square on the Rubik's Cube is assigned a number so that movement can be understood computationally

Yellow (Top face) -> 0-8
Blue (Left face) -> 9-17
Red (Front face) -> 18-26
Green(Right face) -> 27-35
Orange (Back face) -> 36-44
White (Down face) -> 45-53

Total: 9 x 6 = 54 squares


*/

// U move, (Yellow face, clockwise)
const Move U = [] {
    auto p = makeIdentity();

    // U face rotation
    addCycle(p, { 0,2,8,6 });
    addCycle(p, { 1,5,7,3 });

    // side ring (F ? R ? B ? L)
    addCycle(p, { 18,27,36,43 });
    addCycle(p, { 19,28,37,10 });
    addCycle(p, { 20,29,38,11 });

    return Move{ p };
    }();

// R move, (Green face, clockwise)
const Move R = [] {
    auto p = makeIdentity();

    // R face rotation
    addCycle(p, { 27,29,35,33 });
    addCycle(p, { 28,32,34,30 });

    // vertical side cycle
    // U ? F ? D ? B (reverse column)
    addCycle(p, { 2,20,47,42 });
    addCycle(p, { 5,23,50,39 });
    addCycle(p, { 8,26,53,36 });

    return Move{ p };
    }();


const Move F = [] {
    auto p = makeIdentity();

    //F face rotation (red)
    addCycle(p, { 18, 20, 26, 24 });
    addCycle(p, { 19, 23, 25, 21 });

    addCycle(p, { 6, 11, 45, 32 });
    addCycle(p, { 7, 14, 46, 30 });
    addCycle(p, { 8, 17, 47, 27 });

    return Move{ p };
    }();

const Move B = [] {
    auto p = makeIdentity();

    addCycle(p, { 36, 38, 44, 42 });
    addCycle(p, { 37, 41, 43, 39 });

    addCycle(p, { 0, 29, 53, 15 });
    addCycle(p, { 1, 32, 52, 12 });
    addCycle(p, { 2, 35, 51, 9 });

    return Move{ p };
    }();


const Move U_prime = U.inverse();
const Move R_prime = R.inverse();
const Move F_prime = F.inverse();
const Move B_prime = B.inverse();