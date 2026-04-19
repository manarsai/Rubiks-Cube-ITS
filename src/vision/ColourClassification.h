#pragma once
#include <opencv2/opencv.hpp>
#include "../core/domain/Cube.h"


class ColourClassification {
public:
    Colour classify(const cv::Mat& roi);

    std::string toString(Colour colour);
    cv::Mat extractROI(const cv::Mat& frame, const cv::Rect& r);

private:
    int median(std::vector<uchar>& vals);
};

