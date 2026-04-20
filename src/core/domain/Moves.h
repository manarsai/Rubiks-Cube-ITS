#pragma once
#include <array>
#include <vector>

// =========================
// MOVE STRUCTURE
// =========================
struct Moves {
    std::array<int, 54> perm;

    Moves inverse() const {
        Moves inv{};
        for (int i = 0; i < 54; ++i)
            inv.perm[perm[i]] = i;
        return inv;
    }

    friend Moves operator*(const Moves& a, const Moves& b) {
        Moves result{};
        for (int i = 0; i < 54; ++i)
            result.perm[i] = a.perm[b.perm[i]];
        return result;
    }
};

inline bool isIdentity(const Moves& m)
{
    for (int i = 0; i < 54; ++i)
        if (m.perm[i] != i) return false;
    return true;
}

// =========================
// MOVES
// =========================
extern const Moves U;
extern const Moves R;
extern const Moves F;
extern const Moves B;

extern const Moves U_prime;
extern const Moves R_prime;
extern const Moves F_prime;
extern const Moves B_prime;