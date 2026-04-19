#include <iostream>
#include "Solver.h"
#include "Moves.h"
#include "Cube.h"
#include <queue>
#include <unordered_set>
#include <vector>

// node
struct Node {
    std::array<Colour, 54> state{};
    int g = 0;
    int h = 0;
    std::vector<std::string> path;
};

// priority queue
struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return (a.g + a.h) > (b.g + b.h);
    }
};

// solved state
std::array<Colour, 54> solvedState = []() {
    Cube c;
    return c.getState();
    }();

//goal test
bool isSolved(const std::array<Colour, 54>& s) {
    for (int i = 0; i < 54; i++)
        if (s[i] != solvedState[i]) return false;
    return true;
}

// placeholder heuristic
int Solver::heuristic(const std::array<Colour, 54>& s)
{
    int h = 0;

    for (int i = 0; i < 54; i++) {
        if (s[i] != solvedState[i]) h++;
    }

    return h;
}

// apply a move
// perm[i] is where sticker i is coming from
std::array<Colour, 54> applyMove(
    const std::array<Colour, 54>& state,
    const std::array<int, 54>& perm
) {
    std::array<Colour, 54> next;

    for (int i = 0; i < 54; i++) {
        next[i] = state[perm[i]];
    }

    return next;
}

// a* algorithm
std::vector<std::string> Solver::solveWhiteCross(const Cube& cube) {

    std::priority_queue<Node, std::vector<Node>, Compare> open;
    std::unordered_set<std::string> visited;

    Node start;
    start.state = cube.getState();
    start.g = 0;
    start.h = heuristic(start.state);

    open.push(start);

    // rubik's cube movements, notation
    std::vector<std::pair<std::string, std::array<int, 54>>> moves = {
        {"U", U.perm},
        {"R", R.perm},
        {"U'", U_prime.perm},
        { "R'", R_prime.perm },
        { "F", F.perm},
        { "F'", F_prime.perm},
        { "B", B.perm},
        {"B'", B_prime.perm}
    };

    while (!open.empty()) {

        Node current = open.top();
        open.pop();

        // visited key
        std::string key;
        key.reserve(200);

        for (auto c : current.state) {
            key += std::to_string((int)c) + ",";
        }

        if (visited.count(key)) continue;
        visited.insert(key);

        // debugging
        std::cout << "g=" << current.g
            << " h=" << current.h
            << " path=" << current.path.size() << "\n";

        // goal check 
        if (isSolved(current.state)) {
            return current.path;
        }

        // safety limit
        if (current.g > 10) continue;

        // expand
        for (const auto& [name, perm] : moves) {

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