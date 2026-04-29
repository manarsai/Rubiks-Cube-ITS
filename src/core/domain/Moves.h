#pragma once

#include <array>
#include <vector>
#include <string>
#include "Types.h"

struct Move
{
    std::array<int, 54> perm{};

    Move inverse() const
    {
        Move inv{};
        for (int i = 0; i < 54; ++i)
            inv.perm[perm[i]] = i;
        return inv;
    }

    friend Move operator*(const Move& a, const Move& b)
    {
        Move result{};
        for (int i = 0; i < 54; ++i)
            result.perm[i] = a.perm[b.perm[i]];
        return result;
    }
};

// =========================
// BASIC MOVE DEFINITIONS
// =========================
extern const Move U;
extern const Move R;
extern const Move F;
extern const Move B;
extern const Move D;
extern const Move L;

extern const Move U_prime;
extern const Move R_prime;
extern const Move F_prime;
extern const Move B_prime;
extern const Move D_prime;
extern const Move L_prime;

// =========================
// ?? SOLVER MOVE LIST (ADDED)
// =========================
// This replaces per-file movesets in solver.cpp

extern const std::vector<std::pair<std::string, Move>> MOVES;