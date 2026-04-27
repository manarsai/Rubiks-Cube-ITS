#include "Moves.h"
#include <array>
#include <vector>


// IDENTITY
static std::array<int, 54> makeIdentity()
{
    std::array<int, 54> p{};
    for (int i = 0; i < 54; i++)
        p[i] = i;
    return p;
}


// CYCLE HELPER
static void addCycle(std::array<int, 54>& p, const std::vector<int>& c)
{
    for (size_t i = 0; i < c.size(); i++)
    {
        p[c[i]] = c[(i + 1) % c.size()];
    }
}

/*
index layout:
0–8   U (Yellow)
9–17  L (Blue)
18–26 F (Red)
27–35 R (Green)
36–44 B (Orange)
45–53 D (White)
*/

// u move (up face, clockwise)
const Move U = [] {
    auto p = makeIdentity();

    // reverse cycle direction
    addCycle(p, { 0, 6, 8, 2 });
    addCycle(p, { 1, 3, 7, 5 });

    addCycle(p, { 18, 27, 36, 9 });
    addCycle(p, { 19, 28, 37, 10 });
    addCycle(p, { 20, 29, 38, 11 });

    return Move{ p };
    }();

// r move, (right face, clockwise)
const Move R = [] {
    auto p = makeIdentity();

    addCycle(p, { 27, 33, 35, 29 });
    addCycle(p, { 28, 30, 34, 32 });

    addCycle(p, { 2, 20, 47, 42 });
    addCycle(p, { 5, 23, 50, 39 });
    addCycle(p, { 8, 26, 53, 36 });

    return Move{ p };
    }();

// f move, (front face, clockwise)
const Move F = [] {
    auto p = makeIdentity();

    addCycle(p, { 18, 24, 26, 20 });
    addCycle(p, { 19, 21, 25, 23 });

    addCycle(p, { 6, 17, 47, 27 });
    addCycle(p, { 7, 14, 46, 30 });
    addCycle(p, { 8, 11, 45, 33 });

    return Move{ p };
    }();

// b move, (back face, clockwise)
const Move B = [] {
    auto p = makeIdentity();

    addCycle(p, { 36, 42, 44, 38 });
    addCycle(p, { 37, 39, 43, 41 });

    addCycle(p, { 0, 29, 53, 15 });
    addCycle(p, { 1, 32, 52, 12 });
    addCycle(p, { 2, 35, 51, 9 });

    return Move{ p };
    }();

const Move D = [] {
    auto p = makeIdentity();

    addCycle(p, { 45, 51, 53, 47 });
    addCycle(p, { 46, 48, 52, 50 });

    addCycle(p, { 24, 15, 42, 33 });
    addCycle(p, { 25, 16, 43, 34 });
    addCycle(p, { 26, 17, 44, 35 });

    return Move{ p };
    }();

//const Move L = [] {
//    auto p = makeIdentity();
//
//    addCycle(p, { 45, 51, 53, 47 });
//    addCycle(p, { 46, 48, 52, 50 });
//
//    addCycle(p, { 0, 29, 53, 15 });
//    addCycle(p, { 1, 32, 52, 12 });
//    addCycle(p, { 2, 35, 51, 9 });
//
//    return Move{ p };
//    }();

// inverse moves
const Move U_prime = U.inverse();
const Move R_prime = R.inverse();
const Move F_prime = F.inverse();
const Move B_prime = B.inverse();
const Move D_prime = D.inverse();
//const Move L_prime = L.inverse();