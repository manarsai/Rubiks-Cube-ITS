#pragma once
#include <vector>
#include "../core/domain/Cube.h"
#include "DetectedSquare.h"

class FaceBuilder {
public:
    std::vector<DetectedSquare> sortFace(
        const std::vector<DetectedSquare>& input);

};