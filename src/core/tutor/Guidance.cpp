#include "Guidance.h"

// =====================================================
// MAIN GUIDANCE GENERATOR (SAFE)
// =====================================================
std::string Guidance::generate(Stage stage, const Student& student)
{
    double rate = 0.0;

    // -----------------------------------------------------
    // SAFETY: prevent crashes from Student model
    // -----------------------------------------------------
    try
    {
        rate = student.getSuccessRate(stage);
    }
    catch (...)
    {
        rate = 0.0;
    }

    switch (stage)
    {
        // =====================================================
        // SCRAMBLED (invalid / start state)
        // =====================================================
    case Stage::SCRAMBLED:
        return "Start by identifying the white center and building a white cross around it.";

        // =====================================================
        // WHITE CROSS
        // =====================================================
    case Stage::WHITE_CROSS:
        if (rate < 0.4)
        {
            return "Start by finding white edge pieces. Match each edge with its center colour, then move it to the bottom to form a white cross.";
        }

        if (rate < 0.7)
        {
            return "Try solving the white cross while matching edge colours with the center pieces.";
        }

        return "Plan multiple white edges ahead to complete the cross efficiently.";

        // =====================================================
        // F2L
        // =====================================================
    case Stage::F2L:
        if (rate < 0.5)
        {
            return "Find corner-edge pairs and bring them together above their slot.";
        }

        return "Insert paired corner and edge pieces into the correct slot efficiently.";

        // =====================================================
        // OLL
        // =====================================================
    case Stage::OLL:
        if (rate < 0.5)
        {
            return "Focus on orienting yellow pieces on the top face.";
        }

        return "Use algorithms to orient all last layer pieces to make the top uniform.";

        // =====================================================
        // PLL
        // =====================================================
    case Stage::PLL:
        return "Rearrange the last layer pieces to solve the cube completely.";

        // =====================================================
        // DEFAULT SAFETY
        // =====================================================
    default:
        return "Keep going step by step. Focus on one layer at a time.";
    }
}

// =====================================================
// HINT SYSTEM (optional extension)
// =====================================================
std::string Guidance::generateHint(Stage stage, const Student& student)
{
    double rate = 0.0;

    try
    {
        rate = student.getSuccessRate(stage);
    }
    catch (...)
    {
        rate = 0.0;
    }

    switch (stage)
    {
    case Stage::WHITE_CROSS:
        return (rate < 0.5)
            ? "Look for white edge pieces that match center colours."
            : "Try solving two edges before reorienting the cube.";

    case Stage::F2L:
        return "Try lifting corner-edge pairs above their target slot.";

    case Stage::OLL:
        return "Check yellow pattern before applying algorithms.";

    case Stage::PLL:
        return "Identify which pieces are already in correct position.";

    default:
        return "Focus on the current layer only.";
    }
}