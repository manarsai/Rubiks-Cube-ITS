#include "StageDefinitions.h"
#include <iostream>


// BASIC UTIL: CROSS CHECK
static bool isWhiteCrossComplete(const Cube& cube)
{
    const auto& s = cube.getState();

    Colour leftCenter = s[13];
    Colour frontCenter = s[22];
    Colour rightCenter = s[31];
    Colour backCenter = s[40];

    if (s[49] != Colour::WHITE)
        return false;

    struct Edge
    {
        int downPos;
        int sidePos;
        Colour expectedSide;
    };

    Edge edges[4] =
    {
        {46, 25, frontCenter},
        {50, 34, rightCenter},
        {52, 43, backCenter},
        {48, 16, leftCenter}
    };

    for (const auto& e : edges)
    {
        if (s[e.downPos] != Colour::WHITE)
            return false;

        if (s[e.sidePos] != e.expectedSide)
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
        return true;
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
        // STRICT ORDER CHECK (top-down)

        if (validateStage(Stage::PLL, cube))
            return Stage::COMPLETE;

        if (validateStage(Stage::OLL, cube))
            return Stage::PLL;

        if (validateStage(Stage::F2L, cube))
            return Stage::OLL;

        if (validateStage(Stage::WHITE_CROSS, cube))
            return Stage::F2L;

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