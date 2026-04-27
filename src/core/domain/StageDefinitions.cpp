#include "StageDefinitions.h"
#include <iostream>


// BASIC UTIL: CROSS CHECK
static bool isWhiteCrossComplete(const Cube& cube)
{
    // down center
    if (cube.at(DOWN, 1, 1) != Colour::WHITE)
        return false;

    // check edges + alignment
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

// PLACEHOLDER: FUTURE STAGES
static bool isF2LComplete(const Cube& cube)
{
    // TODO: implement properly
    return false;
}

static bool isOLLComplete(const Cube& cube)
{
    // TODO
    return false;
}

static bool isPLLComplete(const Cube& cube)
{
    // TODO
    return false;
}


// STAGE VALIDATION WRAPPER
static bool validateStage(Stage stage, const Cube& cube)
{
    switch (stage)
    {
    case Stage::WHITE_CROSS:
        return isWhiteCrossComplete(cube);

    case Stage::F2L:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube);

    case Stage::OLL:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube) &&
            isOLLComplete(cube);

    case Stage::PLL:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube) &&
            isOLLComplete(cube) &&
            isPLLComplete(cube);

    case Stage::COMPLETE:
        return isWhiteCrossComplete(cube) &&
            isF2LComplete(cube) &&
            isOLLComplete(cube) &&
            isPLLComplete(cube);

    case Stage::SCRAMBLED:
    default:
        return false;
    }
}

// STAGE DEFINITIONS
static StageDefinition WHITE_CROSS_STAGE =
{
    Stage::WHITE_CROSS,
    "White Cross",
    [](const Cube& cube)
    {
        return validateStage(Stage::WHITE_CROSS, cube);
    }
};

static StageDefinition F2L_STAGE =
{
    Stage::F2L,
    "F2L",
    [](const Cube& cube)
    {
        return validateStage(Stage::F2L, cube);
    }
};

static StageDefinition OLL_STAGE =
{
    Stage::OLL,
    "OLL",
    [](const Cube& cube)
    {
        return validateStage(Stage::OLL, cube);
    }
};

static StageDefinition PLL_STAGE =
{
    Stage::PLL,
    "PLL",
    [](const Cube& cube)
    {
        return validateStage(Stage::PLL, cube);
    }
};

static StageDefinition SCRAMBLED_STAGE =
{
    Stage::SCRAMBLED,
    "SCRAMBLED",
    [](const Cube& cube)
    {
        return true;
    }
};


// PUBLIC API
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
        case Stage::WHITE_CROSS:
            return WHITE_CROSS_STAGE;

        case Stage::F2L:
            return F2L_STAGE;

        case Stage::OLL:
            return OLL_STAGE;

        case Stage::PLL:
            return PLL_STAGE;

        case Stage::COMPLETE:
            return PLL_STAGE;

        case Stage::SCRAMBLED:
        default:
            return SCRAMBLED_STAGE;
        }
    }
}