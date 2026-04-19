#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <array>

#include "../core/domain/Cube.h"

#include "FrameProcessing.h"
#include "DetectCubeSquares.h"
#include "ColourClassification.h"
#include "FaceBuilder.h"
#include "DetectedSquare.h"


class Vision {
public:
    Vision(int cameraIndex = 0);
    ~Vision();



    // Main pipeline
    cv::Mat getProcessedFrame();

    // Final output for solver / cube model
    std::array<Colour, 9> getFaceColours();

    // Optional debug access
    cv::Mat frame;
    std::vector<std::vector<cv::Point>> lastContours;
    std::vector<DetectedSquare> lastDetectedFace;

private:
    // Processing modules
    FrameProcessing processor;
    DetectCubeSquares detector;
    ColourClassification classifier;
    FaceBuilder faceBuilder;

    cv::VideoCapture cap;
};

#endif