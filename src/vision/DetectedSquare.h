#pragma once

#include <opencv2/opencv.hpp>
#include "../core/domain/Cube.h"

struct DetectedSquare {
    cv::Point center;
    Colour colour;
};