#pragma once
#include <array>

struct Move {
    std::array<int, 54> perm;

    // composition: apply b first, then a
    friend Move operator*(const Move& a, const Move& b) {
        Move result{};

        for (int i = 0; i < 54; ++i) {
            result.perm[i] = a.perm[b.perm[i]];
        }

        return result;
    }

    Move inverse() const {
        Move inv{};
        for (int i = 0; i < 54; ++i) {
            inv.perm[perm[i]] = i;
        }
        return inv;
    }
};

inline bool isIdentity(const Move& m) {
    for (int i = 0; i < 54; ++i) {
        if (m.perm[i] != i) return false;
    }
    return true;
}

extern const Move U;
extern const Move R;
extern const Move F;
extern const Move B;

extern const Move U_prime;
extern const Move R_prime;
extern const Move F_prime;
extern const Move B_prime;