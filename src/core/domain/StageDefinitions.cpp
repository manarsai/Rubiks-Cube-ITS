#include "StageDefinitions.h"
#include "FaceDirections.h"
#include "Types.h"

// =========================
// INDEX HELPER
// =========================
static inline int idx(int face, int r, int c)
{
    return face * 9 + r * 3 + c;
}

// =====================================================
// WHITE CROSS
// =====================================================
bool StageDefinitions::isWhiteCrossComplete(const State& s)
{
    if (s[idx(DOWN, 1, 1)] != Colour::WHITE)
        return false;

    struct Edge
    {
        int drow, dcol;
        int sideFace;
        Colour expectedSide;
    };

    Edge edges[] =
    {
        {0, 1, FRONT, Colour::RED},
        {1, 2, RIGHT, Colour::GREEN},
        {1, 0, LEFT,  Colour::BLUE},
        {2, 1, BACK,  Colour::ORANGE}
    };

    for (const auto& e : edges)
    {
        if (s[idx(DOWN, e.drow, e.dcol)] != Colour::WHITE)
            return false;

        if (s[idx(e.sideFace, 2, 1)] != e.expectedSide)
            return false;
    }

    return true;
}

// =====================================================
// F2L
// =====================================================
bool StageDefinitions::isF2LComplete(const State& s)
{
    // Must at least have white cross
    if (!isWhiteCrossComplete(s))
        return false;

    // F2L check = bottom two layers filled (white layer done)
    for (int face = 0; face < 4; face++)
    {
        for (int r = 0; r < 2; r++)   // ONLY bottom 2 layers
        {
            for (int c = 0; c < 3; c++)
            {
                if (s[idx(face, r, c)] == Colour::WHITE)
                    return false;
            }
        }
    }

    return true;
}

// =====================================================
// OLL
// =====================================================
bool StageDefinitions::isOLLComplete(const State& s)
{
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (s[idx(UP, r, c)] != Colour::YELLOW)
                return false;

    return true;
}

// =====================================================
// PLL (FULL CUBE SOLVED CHECK)
// =====================================================
bool StageDefinitions::isPLLComplete(const State& s)
{
    for (int face = 0; face < 6; face++)
    {
        Colour center = s[idx(face, 1, 1)];

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (s[idx(face, r, c)] != center)
                    return false;
    }

    return true;
}

// =====================================================
// VALIDATION (FIXED LOGIC FLOW)
// =====================================================
bool StageDefinitions::validateStage(Stage stage, const State& s)
{
    switch (stage)
    {
    case Stage::WHITE_CROSS:
        return isWhiteCrossComplete(s);

    case Stage::F2L:
        return isWhiteCrossComplete(s) && isF2LComplete(s);

    case Stage::OLL:
        return isF2LComplete(s) && isOLLComplete(s);

    case Stage::PLL:
        return isF2LComplete(s)
            && isOLLComplete(s)
            && isPLLComplete(s);

    case Stage::COMPLETE:
        return isPLLComplete(s);

    default:
        return false;
    }
}

// =====================================================
// STAGE DETECTION (FIXED ORDER - MOST IMPORTANT)
// =====================================================
Stage StageDefinitions::detect(const State& s)
{
    if (isPLLComplete(s)) return Stage::COMPLETE;
    if (isOLLComplete(s)) return Stage::OLL;
    if (isF2LComplete(s)) return Stage::F2L;
    if (isWhiteCrossComplete(s)) return Stage::WHITE_CROSS;

    return Stage::SCRAMBLED; // ?? IMPORTANT
}

// =====================================================
// STAGE DEFINITIONS
// =====================================================
static StageDefinitions::StageDefinition WHITE_CROSS_STAGE =
{
    Stage::WHITE_CROSS,
    "White Cross",
    [](const State& s)
    {
        return StageDefinitions::validateStage(Stage::WHITE_CROSS, s);
    }
};

static StageDefinitions::StageDefinition F2L_STAGE =
{
    Stage::F2L,
    "F2L",
    [](const State& s)
    {
        return StageDefinitions::validateStage(Stage::F2L, s);
    }
};

static StageDefinitions::StageDefinition OLL_STAGE =
{
    Stage::OLL,
    "OLL",
    [](const State& s)
    {
        return StageDefinitions::validateStage(Stage::OLL, s);
    }
};

static StageDefinitions::StageDefinition PLL_STAGE =
{
    Stage::PLL,
    "PLL",
    [](const State& s)
    {
        return StageDefinitions::validateStage(Stage::PLL, s);
    }
};

static StageDefinitions::StageDefinition SCRAMBLED_STAGE =
{
    Stage::SCRAMBLED,
    "SCRAMBLED",
    [](const State&)
    {
        return false;
    }
};

// =====================================================
// PUBLIC API
// =====================================================
namespace StageDefinitions
{
    const StageDefinition& get(Stage stage)
    {
        switch (stage)
        {
        case Stage::WHITE_CROSS: return WHITE_CROSS_STAGE;
        case Stage::F2L:         return F2L_STAGE;
        case Stage::OLL:         return OLL_STAGE;
        case Stage::PLL:         return PLL_STAGE;
        case Stage::COMPLETE:    return PLL_STAGE; // solved cube == PLL complete
        default:                 return SCRAMBLED_STAGE;
        }
    }
}