#include "Guidance.h"

std::string Guidance::generate(Stage stage, const Student& student)
{
    double rate = student.getSuccessRate(stage);

    switch (stage)
    {
    case Stage::WHITE_CROSS:

        if (rate < 0.4)
        {
            return "Start by finding white edge pieces. Match each edge with its center colour, then move it to the bottom to form a white cross.";
        }

        if (rate < 0.7)
        {
            return "Try solving the white cross while matching edge colours with the center pieces.";
        }

        return "Complete the white cross efficiently by planning multiple edges ahead.";

    case Stage::F2L:
        return "Pair corner and edge pieces and insert them into the correct slot.";

    case Stage::OLL:
        return "Orient all last layer pieces so the top face becomes one colour.";

    case Stage::PLL:
        return "Permute the last layer pieces to finish the cube.";

    default:
        return "Keep going.";
    }
}