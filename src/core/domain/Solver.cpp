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
// HEURISTIC (slightly improved)
// =========================
int Solver::heuristic(const std::array<Colour, 54>& s)
{
    int h = 0;

    int edges[4] = { 46, 50, 52, 48 };

    for (int i : edges)
    {
        if (s[i] != Colour::WHITE)
            h += 2; // stronger penalty improves ordering
    }

    return h;
}

// =========================
// STATE HASH
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

    // =========================
    // FULL MOVE SET (FIXED)
    // =========================
    std::vector<std::pair<std::string, Move>> moves =
    {
        {"U",  U}, {"U'", U_prime},
        {"D",  D}, {"D'", D_prime},
        {"F",  F}, {"F'", F_prime},
        {"B",  B}, {"B'", B_prime},
        {"R",  R}, {"R'", R_prime}
    };

    // =========================
    // SEARCH LOOP
    // =========================
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
        // DEPTH LIMIT (FIXED)
        // =========================
        if (current.g >= 15)   // ?? increased from 8 ? 15
            continue;

        // =========================
        // EXPAND
        // =========================
        for (const auto& [name, move] : moves)
        {
            // =========================
            // PRUNING (SAFE VERSION ONLY)
            // =========================
            if (!current.path.empty())
            {
                const std::string& last = current.path.back();

                // prevent direct undo
                if ((last == "U" && name == "U'") ||
                    (last == "U'" && name == "U") ||
                    (last == "F" && name == "F'") ||
                    (last == "F'" && name == "F") ||
                    (last == "R" && name == "R'") ||
                    (last == "R'" && name == "R") ||
                    (last == "D" && name == "D'") ||
                    (last == "D'" && name == "D") ||
                    (last == "B" && name == "B'") ||
                    (last == "B'" && name == "B"))
                {
                    continue;
                }
            }

            // =========================
            // APPLY MOVE (FAST VERSION)
            // =========================
            Node next;
            next.state = current.state; // copy

            Cube temp;
            temp.setState(current.state);
            temp.applyMove(move);
            next.state = temp.getState();

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

    Cube temp;
    temp.setState(original);
    temp.applyMove(U);

    Cube temp2;
    temp2.setState(temp.getState());
    temp2.applyMove(U_prime);

    bool ok = (temp2.getState() == original);

    std::cout << "U -> U' test: " << (ok ? "PASS" : "FAIL") << "\n";

    if (!ok)
        std::cout << "ERROR: inverse moves are NOT correct\n";

    std::cout << "=============================\n";
}