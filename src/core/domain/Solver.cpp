#include <iostream>
#include "Solver.h"
#include "Moves.h"
#include "Cube.h"
#include "StageDefinitions.h"
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
// RECOVERY TARGET
// =========================
static Stage getRecoveryTarget(Stage current)
{
    switch (current)
    {
    case Stage::WHITE_CROSS: return Stage::WHITE_CROSS;
    case Stage::F2L:         return Stage::WHITE_CROSS;
    case Stage::OLL:         return Stage::F2L;
    case Stage::PLL:         return Stage::OLL;
    default:                 return Stage::WHITE_CROSS;
    }
}

// =========================
// HEURISTIC (simple but stage-aware)
// =========================
int Solver::heuristic(const std::array<Colour, 54>& s, Stage stage)
{
    Cube temp;
    temp.setState(s);

    if (StageDefinitions::validateStage(stage, temp))
        return 0;

    return 10; // simple fallback
}

// =========================
// GENERIC STAGE SOLVER
// =========================
std::vector<std::string> Solver::solveToStage(const Cube& cube, Stage targetStage)
{
    std::priority_queue<Node, std::vector<Node>, Compare> open;
    std::unordered_set<size_t> visited;
    StateHash hasher;

    Node start;
    start.state = cube.getState();
    start.g = 0;
    start.h = heuristic(start.state, targetStage);

    open.push(start);

    // =========================
    // MOVE SET
    // =========================
    std::vector<std::pair<std::string, Move>> moves =
    {
        {"U",  U}, {"U'", U_prime},
        {"D",  D}, {"D'", D_prime},
        {"F",  F}, {"F'", F_prime},
        {"B",  B}, {"B'", B_prime},
        {"R",  R}, {"R'", R_prime},
        //{"L",  L}, {"L'", L_prime} // ? added missing L moves
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

        Cube temp;
        temp.setState(current.state);

        // =========================
        // GOAL CHECK
        // =========================
        if (StageDefinitions::validateStage(targetStage, temp))
        {
            std::cout << "Reached stage: " << (int)targetStage << "\n";
            return current.path;
        }

        // =========================
        // DEPTH LIMIT (safety)
        // =========================
        if (current.g >= 20)
            continue;

        // =========================
        // EXPAND
        // =========================
        for (const auto& [name, move] : moves)
        {
            // avoid immediate inverse moves
            if (!current.path.empty())
            {
                const std::string& last = current.path.back();

                if ((last == "U" && name == "U'") || (last == "U'" && name == "U") ||
                    (last == "D" && name == "D'") || (last == "D'" && name == "D") ||
                    (last == "F" && name == "F'") || (last == "F'" && name == "F") ||
                    (last == "B" && name == "B'") || (last == "B'" && name == "B") ||
                    (last == "R" && name == "R'") || (last == "R'" && name == "R") ||
                    (last == "L" && name == "L'") || (last == "L'" && name == "L"))
                {
                    continue;
                }
            }

            Node next;
            next.state = current.state;

            Cube nextCube;
            nextCube.setState(current.state);
            nextCube.applyMove(move);

            next.state = nextCube.getState();
            next.g = current.g + 1;
            next.h = heuristic(next.state, targetStage);
            next.path = current.path;
            next.path.push_back(name);

            open.push(next);
        }
    }

    std::cout << "No solution found for stage recovery.\n";
    return {};
}

// =========================
// RECOVERY ENTRY POINT
// =========================
std::vector<std::string> Solver::recover(const Cube& cube)
{
    Stage current = StageDefinitions::detect(cube);
    Stage target = getRecoveryTarget(current);

    std::cout << "Recovering from stage " << (int)current
        << " ? " << (int)target << "\n";

    return solveToStage(cube, target);
}

// =========================
// DEBUG TEST
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