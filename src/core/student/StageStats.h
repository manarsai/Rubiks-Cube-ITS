#pragma once

struct StageStats
{
    int successCount = 0;
    int failCount = 0;
    int solverUses = 0;
    double timeSpent = 0.0;

    double successRate() const
    {
        int total = successCount + failCount;
        return total ? static_cast<double>(successCount) / total : 0.0;
    }
};