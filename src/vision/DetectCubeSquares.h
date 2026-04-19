#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class DetectCubeSquares {
public:
    // Main pipeline
    std::vector<std::vector<cv::Point>> detect(const cv::Mat& edges);

    // Post-processing steps
    std::vector<std::vector<cv::Point>> filterStableSquares(
        const std::vector<std::vector<cv::Point>>& squares);

    std::vector<std::vector<cv::Point>> removeDuplicates(
        const std::vector<std::vector<cv::Point>>& squares);

private:
    // Core detection pipeline
    std::vector<std::vector<cv::Point>> detectContours(const cv::Mat& edges);

    std::vector<std::vector<cv::Point>> approximateContours(
        const std::vector<std::vector<cv::Point>>& contours);

    bool isSquare(const std::vector<cv::Point>& contour);

private:
    // Temporal tracking memory
    std::vector<cv::Rect> lastSquares;

    // Tuning parameter
    double approxEpsilonFactor = 0.02;
};