#include "FaceBuilder.h"
#include <algorithm>
#include <cmath>

std::vector<DetectedSquare> FaceBuilder::sortFace(
    const std::vector<DetectedSquare>& input)
{
    auto sorted = input;

    std::sort(sorted.begin(), sorted.end(),
        [](const DetectedSquare& a, const DetectedSquare& b)
        {
            if (std::abs(a.center.y - b.center.y) > 25)
                return a.center.y < b.center.y;
            return a.center.x < b.center.x;
        });

    return sorted;
}