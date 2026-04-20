#include "Moves.h"
#include <array>
#include <vector>

// =========================
// IDENTITY
// =========================
static std::array<int, 54> makeIdentity()
{
    std::array<int, 54> p{};
    for (int i = 0; i < 54; i++)
        p[i] = i;
    return p;
}

// =========================
// CYCLE HELPER
// (i0 ? i1 ? i2 ? ... ? i0)
// =========================
static void addCycle(std::array<int, 54>& p, const std::vector<int>& c)
{
    for (size_t i = 0; i < c.size(); i++)
    {
        p[c[i]] = c[(i + 1) % c.size()];
    }
}

/*
Index layout:
0–8   U (Yellow)
9–17  L (Blue)
18–26 F (Red)
27–35 R (Green)
36–44 B (Orange)
45–53 D (White)
*/

// =========================
// U MOVE
// =========================
const Moves U = [] {
    auto p = makeIdentity();

    addCycle(p, { 0, 2, 8, 6 });
    addCycle(p, { 1, 5, 7, 3 });

    addCycle(p, { 18, 27, 36, 9 });
    addCycle(p, { 19, 28, 37, 10 });
    addCycle(p, { 20, 29, 38, 11 });

    return Moves{ p };
    }();

// =========================
// R MOVE
// =========================
const Moves R = [] {
    auto p = makeIdentity();

    addCycle(p, { 27, 29, 35, 33 });
    addCycle(p, { 28, 32, 34, 30 });

    addCycle(p, { 2, 20, 47, 42 });
    addCycle(p, { 5, 23, 50, 39 });
    addCycle(p, { 8, 26, 53, 36 });

    return Moves{ p };
    }();

// =========================
// F MOVE
// =========================
const Moves F = [] {
    auto p = makeIdentity();

    addCycle(p, { 18, 20, 26, 24 });
    addCycle(p, { 19, 23, 25, 21 });

    addCycle(p, { 6, 11, 45, 32 });
    addCycle(p, { 7, 14, 46, 29 });
    addCycle(p, { 8, 17, 47, 27 });

    return Moves{ p };
    }();

// =========================
// B MOVE
// =========================
const Moves B = [] {
    auto p = makeIdentity();

    addCycle(p, { 36, 38, 44, 42 });
    addCycle(p, { 37, 41, 43, 39 });

    addCycle(p, { 0, 29, 53, 15 });
    addCycle(p, { 1, 32, 52, 12 });
    addCycle(p, { 2, 35, 51, 9 });

    return Moves{ p };
    }();

// =========================
// INVERSES
// =========================
const Moves U_prime = U.inverse();
const Moves R_prime = R.inverse();
const Moves F_prime = F.inverse();
const Moves B_prime = B.inverse();