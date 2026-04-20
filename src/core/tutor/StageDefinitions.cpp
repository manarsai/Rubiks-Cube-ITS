#include "StageDefinitions.h"
#include <iostream>

// =====================================================
// WHITE CROSS CHECK (TUTOR-GRADE + STD::COUT DEBUG)
// =====================================================
static bool isWhiteCrossComplete(const Cube& cube)
{
    const auto& s = cube.getState();

    std::cout << "\n==============================\n";
    std::cout << "WHITE CROSS CHECK START\n";
    std::cout << "==============================\n";

    // ===============================
    // CENTERS (reference colours)
    // ===============================
    Colour leftCenter = s[13];
    Colour frontCenter = s[22];
    Colour rightCenter = s[31];
    Colour backCenter = s[40];
    Colour downCenter = s[49];

    std::cout << "Centers:\n";
    std::cout << "Left  : " << (int)leftCenter << "\n";
    std::cout << "Front : " << (int)frontCenter << "\n";
    std::cout << "Right : " << (int)rightCenter << "\n";
    std::cout << "Back  : " << (int)backCenter << "\n";
    std::cout << "Down  : " << (int)downCenter << "\n";

    // Must have white center on bottom
    if (downCenter != Colour::WHITE)
    {
        std::cout << "FAIL: Down center is not WHITE\n";
        return false;
    }

    // ===============================
    // EDGE DEFINITIONS
    // ===============================
    struct Edge
    {
        const char* name;
        int downPos;
        int sidePos;
        Colour expectedSide;
    };

    Edge edges[4] =
    {
        { "FRONT", 46, 25, frontCenter },
        { "RIGHT", 50, 34, rightCenter },
        { "BACK",  52, 43, backCenter },
        { "LEFT",  48, 16, leftCenter }
    };

    // ===============================
    // CHECK EACH EDGE
    // ===============================
    for (const auto& e : edges)
    {
        Colour downColour = s[e.downPos];
        Colour sideColour = s[e.sidePos];

        std::cout << "\nChecking: " << e.name << "\n";
        std::cout << "Down pos " << e.downPos << " = " << (int)downColour << "\n";
        std::cout << "Side pos " << e.sidePos << " = " << (int)sideColour << "\n";
        std::cout << "Expected side center = " << (int)e.expectedSide << "\n";

        if (downColour != Colour::WHITE)
        {
            std::cout << "FAIL: " << e.name << " down sticker not WHITE\n";
            return false;
        }

        if (sideColour != e.expectedSide)
        {
            std::cout << "FAIL: " << e.name << " side mismatch\n";
            return false;
        }
    }

    std::cout << "\nSUCCESS: WHITE CROSS COMPLETE\n";
    std::cout << "==============================\n\n";

    return true;
}

// =====================================================
// STAGE DEFINITIONS
// =====================================================
static StageDefinition WHITE_CROSS_STAGE =
{
    Stage::WHITE_CROSS,
    "White Cross",
    isWhiteCrossComplete
};

static StageDefinition F2L_STAGE =
{
    Stage::F2L,
    "F2L",
    nullptr
};

static StageDefinition OLL_STAGE =
{
    Stage::OLL,
    "OLL",
    nullptr
};

static StageDefinition PLL_STAGE =
{
    Stage::PLL,
    "PLL",
    nullptr
};

// =====================================================
// LOOKUP
// =====================================================
namespace StageDefinitions
{
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

        default:
            return WHITE_CROSS_STAGE;
        }
    }
}