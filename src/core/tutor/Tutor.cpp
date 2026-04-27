#include "Tutor.h"

Tutor::Tutor() {}

Stage Tutor::decideNextStage(
    Stage expected,
    Stage actual,
    const Student& student)
{
    auto stats = student.getStats(expected);
    double rate = student.getSuccessRate(expected);

    // =====================================================
    // 1. If user is already ahead ? trust them
    // =====================================================
    if (actual > expected)
        return actual;

    // =====================================================
    // 2. If struggling ? stay on same stage
    // =====================================================
    if (rate < 0.5 || stats.failCount > 3)
        return expected;

    // =====================================================
    // 3. Otherwise ? move forward ONE stage
    // =====================================================
    switch (expected)
    {
    case Stage::WHITE_CROSS: return Stage::F2L;
    case Stage::F2L:         return Stage::OLL;
    case Stage::OLL:         return Stage::PLL;
    case Stage::PLL:         return Stage::COMPLETE;
    default:                 return expected;
    }
}