#include <iostream>
#include "Solver.h"
#include "Moves.h"
#include "Cube.h"
#include <queue>
#include <unordered_set>
#include <vector>

// =========================
// NODE
// =========================
struct Node {
    std::array<Colour, 54> state{};
    int g = 0;
    int h = 0;
    std::vector<std::string> path;
};

// =========================
// PRIORITY QUEUE
// =========================
struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return (a.g + a.h) > (b.g + b.h);
    }
};

// =========================
// APPLY MOVE (FIXED DIRECTION)
// =========================
// perm[i] = destination of sticker i
std::array<Colour, 54> applyMove(
    const std::array<Colour, 54>& state,
    const std::array<int, 54>& perm
) {
    std::array<Colour, 54> next{};

    for (int i = 0; i < 54; i++)
        next[perm[i]] = state[i];

    return next;
}

// =========================
// GOAL: WHITE CROSS ONLY
// =========================
bool isWhiteCross(const std::array<Colour, 54>& s)
{
    int edges[4] = { 46, 50, 52, 48 };

    for (int i : edges)
        if (s[i] != Colour::WHITE)
            return false;

    return true;
}

// =========================
// HEURISTIC (CROSS ONLY)
// =========================
int Solver::heuristic(const std::array<Colour, 54>& s)
{
    int h = 0;

    int edges[4] = { 46, 50, 52, 48 };

    for (int i : edges)
        if (s[i] != Colour::WHITE)
            h++;

    return h;
}

// =========================
// STATE HASH (FAST VISITED)
// =========================
struct StateHash {
    size_t operator()(const std::array<Colour, 54>& s) const {
        size_t h = 0;
        for (auto c : s)
            h = h * 31 + static_cast<int>(c);
        return h;
    }
};

// =========================
// SOLVER
// =========================
std::vector<std::string> Solver::solveWhiteCross(const Cube& cube)
{
    std::priority_queue<Node, std::vector<Node>, Compare> open;
    std::unordered_set<size_t> visited;

    StateHash hasher;

    Node start;
    start.state = cube.getState();
    start.g = 0;
    start.h = heuristic(start.state);

    open.push(start);

    // Reduced but sufficient move set
    std::vector<std::pair<std::string, std::array<int, 54>>> moves =
    {
        {"U",  U.perm},
        {"U'", U_prime.perm},
        {"F",  F.perm},
        {"F'", F_prime.perm},
        {"R",  R.perm},
        {"R'", R_prime.perm}
    };

    while (!open.empty())
    {
        Node current = open.top();
        open.pop();

        size_t key = hasher(current.state);

        if (visited.count(key))
            continue;

        visited.insert(key);

        // =========================
        // GOAL CHECK
        // =========================
        if (isWhiteCross(current.state))
        {
            std::cout << "WHITE CROSS FOUND\n";
            return current.path;
        }

        // =========================
        // DEPTH LIMIT (cross is shallow)
        // =========================
        if (current.g >= 8)
            continue;

        // =========================
        // EXPAND
        // =========================
        for (const auto& [name, perm] : moves)
        {
            // avoid immediate reversals
            if (!current.path.empty())
            {
                const std::string& last = current.path.back();
                if ((last == "U" && name == "U'") ||
                    (last == "U'" && name == "U") ||
                    (last == "F" && name == "F'") ||
                    (last == "F'" && name == "F") ||
                    (last == "R" && name == "R'") ||
                    (last == "R'" && name == "R"))
                {
                    continue;
                }
            }

            Node next;
            next.state = applyMove(current.state, perm);
            next.g = current.g + 1;
            next.h = heuristic(next.state);
            next.path = current.path;
            next.path.push_back(name);

            open.push(next);
        }
    }

    return {};
}


// =========================
// DEBUG: MOVE INVERSE TEST
// =========================
void Solver::testMoveInverses()
{
    std::cout << "\n=== TESTING MOVE INVERSES ===\n";

    Cube c;

    auto original = c.getState();

    // apply move then inverse
    auto afterU = applyMove(original, U.perm);
    auto backU = applyMove(afterU, U_prime.perm);

    bool ok = (backU == original);

    std::cout << "U -> U' test: " << (ok ? "PASS" : "FAIL") << "\n";

    if (!ok)
    {
        std::cout << "ERROR: inverse moves are NOT correct\n";
    }

    std::cout << "=============================\n";
}