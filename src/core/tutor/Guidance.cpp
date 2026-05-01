#include "Guidance.h"

std::string Guidance::generate(Stage stage, const Student& student)
{
    int support = student.getSupportLevel(stage);

    switch (stage)
    {
    case Stage::SCRAMBLED:
        return         "Welcome to the Rubik's Cube ITS!\n\n"
        "1. Click 'Scan Cube'\n"
        "2. Follow the prompts and scan all six faces.\n"
        "3. Receive guidance based on your cube's state.\n\n"
            "Goal: Solve using CFOP";

    case Stage::WHITE_CROSS:
        if (support == 2)
            return "1. Place the WHITE edge pieces around the YELLOW center. This will form a daisy shape.\n"
                   "2. Look at the colours adjacent to the white edges. Align them witht their corresponding centre piece.\n"
                   "3. For every edge matched with a centre, rotate that face twice.\n";
        if (support == 1)
            return "Match edge colors with center pieces.";
        return "Plan multiple edges ahead.";

    case Stage::F2L:
        if (support == 2)
            return "Pair corner and edge pieces above their slot.";
        if (support == 1)
            return "Insert pairs step-by-step into correct slots.";
        return "Insert pairs efficiently into correct slots.";

    case Stage::OLL:
        if (support == 2)
            return "Orient yellow face pieces on top.";
        if (support == 1)
            return "Use basic algorithms to orient last layer.";
        return "Execute OLL algorithms efficiently.";

    case Stage::PLL:
        if (support == 2)
            return "Permute last layer pieces step-by-step.";
        return "Use PLL algorithms to finish the cube.";

    case Stage::COMPLETE:
        return "Cube solved.";

    default:
        return "Focus on the current stage.";
    }
}