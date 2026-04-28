#include "StageDefinitions.h"

// =====================================================
// WHITE CROSS CHECK
// =====================================================
bool StageDefinitions::isWhiteCrossComplete(const Cube& cube)
{
    if (cube.at(DOWN, 1, 1) != Colour::WHITE)
        return false;

    struct Edge {
        int drow, dcol;
        int sideFace;
        Colour expectedSide;
    };

    Edge edges[] = {
        {0, 1, FRONT, Colour::RED},
        {1, 2, RIGHT, Colour::GREEN},
        {1, 0, LEFT,  Colour::BLUE},
        {2, 1, BACK,  Colour::ORANGE}
    };

    for (const auto& e : edges)
    {
        if (cube.at(DOWN, e.drow, e.dcol) != Colour::WHITE)
            return false;

        if (cube.at(e.sideFace, 2, 1) != e.expectedSide)
            return false;
    }

    return true;
}


bool StageDefinitions::isF2LComplete(const Cube& cube)
{
    // 1. Bottom face (white)
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (cube.at(DOWN, r, c) != Colour::WHITE)
                return false;

    struct FaceCheck {
        int face;
        Colour center;
    };

    FaceCheck faces[] = {
        {FRONT, Colour::RED},
        {RIGHT, Colour::GREEN},
        {BACK,  Colour::ORANGE},
        {LEFT,  Colour::BLUE}
    };

    // 2. Bottom rows match centers
    for (const auto& f : faces)
    {
        for (int col = 0; col < 3; col++)
        {
            if (cube.at(f.face, 2, col) != f.center)
                return false;
        }
    }

    // 3. Middle layer edges
    for (const auto& f : faces)
    {
        if (cube.at(f.face, 1, 0) != f.center)
            return false;

        if (cube.at(f.face, 1, 2) != f.center)
            return false;
    }

    return true;
}

bool StageDefinitions::isOLLComplete(const Cube& cube)
{
    // Check entire UP face is yellow
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            if (cube.at(UP, r, c) != Colour::YELLOW)
                return false;
        }
    }

    return true;
}

bool StageDefinitions::isPLLComplete(const Cube& cube)
{
    for (int face = 0; face < 6; face++)
    {
        Colour center = cube.at(face, 1, 1);

        for (int r = 0; r < 3; r++)
        {
            for (int c = 0; c < 3; c++)
            {
                if (cube.at(face, r, c) != center)
                    return false;
            }
        }
    }

    return true;
}
// =====================================================
// CORE VALIDATOR
// =====================================================
bool StageDefinitions::validateStage(Stage stage, const Cube& cube)
{
    switch (stage)
    {
    case Stage::WHITE_CROSS:
        return isWhiteCrossComplete(cube);

    case Stage::F2L:
        return isWhiteCrossComplete(cube) && isF2LComplete(cube);

    case Stage::OLL:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube) &&
            isOLLComplete(cube);

    case Stage::PLL:
    case Stage::COMPLETE:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube) &&
            isOLLComplete(cube) &&
            isPLLComplete(cube);

    default:
        return false;
    }
}

// =====================================================
// STAGE DEFINITIONS
// =====================================================
static StageDefinition WHITE_CROSS_STAGE =
{
    Stage::WHITE_CROSS,
    "White Cross",
    [](const Cube& cube)
    {
        return StageDefinitions::validateStage(Stage::WHITE_CROSS, cube);
    }
};

static StageDefinition F2L_STAGE =
{
    Stage::F2L,
    "F2L",
    [](const Cube& cube)
    {
        return StageDefinitions::validateStage(Stage::F2L, cube);
    }
};

static StageDefinition OLL_STAGE =
{
    Stage::OLL,
    "OLL",
    [](const Cube& cube)
    {
        return StageDefinitions::validateStage(Stage::OLL, cube);
    }
};

static StageDefinition PLL_STAGE =
{
    Stage::PLL,
    "PLL",
    [](const Cube& cube)
    {
        return StageDefinitions::validateStage(Stage::PLL, cube);
    }
};

static StageDefinition SCRAMBLED_STAGE =
{
    Stage::SCRAMBLED,
    "SCRAMBLED",
    [](const Cube&)
    {
        return false;
    }
};

// =====================================================
// PUBLIC API
// =====================================================
namespace StageDefinitions
{
    Stage detect(const Cube& cube)
    {
        bool cross = isWhiteCrossComplete(cube);
        bool f2l = cross && isF2LComplete(cube);
        bool oll = f2l && isOLLComplete(cube);
        bool pll = oll && isPLLComplete(cube);

        if (pll) return Stage::COMPLETE;
        if (oll) return Stage::PLL;
        if (f2l) return Stage::F2L;
        if (cross) return Stage::WHITE_CROSS;

        return Stage::SCRAMBLED;
    }

    const StageDefinition& get(Stage stage)
    {
        switch (stage)
        {
        case Stage::WHITE_CROSS: return WHITE_CROSS_STAGE;
        case Stage::F2L:         return F2L_STAGE;
        case Stage::OLL:         return OLL_STAGE;
        case Stage::PLL:         return PLL_STAGE;
        case Stage::COMPLETE:    return PLL_STAGE;
        default:                 return SCRAMBLED_STAGE;
        }
    }
}