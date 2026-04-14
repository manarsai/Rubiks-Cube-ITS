#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <array>
#include "../core/domain/Cube.h"

// Enum for cube colors
enum class CubeColour {
    White,
    Red,
    Orange,
    Yellow,
    Green,
    Blue,
    Unknown
};

struct DetectedSquare {
    cv::Point center;
    CubeColour colour;
};

class Vision {
public:
    Vision(int cameraIndex = 0);
    ~Vision();

    // Grab and process a frame
    cv::Mat getProcessedFrame();

    // Access intermediate results
    cv::Mat lastFrame;
    cv::Mat lastGray;
    cv::Mat lastBlur;
    cv::Mat lastEdges;
    cv::Mat lastDilated;
    std::vector<std::vector<cv::Point>> lastContours;

    // Detection result
    std::vector<DetectedSquare> lastDetectedFace;

    // ?? IMPORTANT: these must exist
    std::array<Color, 9> getFaceColors();
    Color convertToCubeColor(CubeColour c);

    // Parameters
    int dilationIterations = 1;
    double approxEpsilonFactor = 0.02;

    CubeColour classifySquareHSV(const cv::Mat& roi);

private:
    cv::VideoCapture cap;

    std::vector<cv::Rect> lastSquares;

    cv::Mat preprocessFrame(const cv::Mat& frame);
    std::vector<std::vector<cv::Point>> detectContours(const cv::Mat& edges);
    std::vector<std::vector<cv::Point>> approximateContours(const std::vector<std::vector<cv::Point>>& contours);
    bool isSquare(const std::vector<cv::Point>& contour);

    std::string cubeColourToString(CubeColour color);
};

#endif