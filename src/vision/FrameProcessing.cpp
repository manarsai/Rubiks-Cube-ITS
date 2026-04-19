#include "FrameProcessing.h"

cv::Mat FrameProcessing::process(const cv::Mat& frame)
{
    cv::Mat bilateral, guassian, canny, morphology, processResult;

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    cv::Mat value = channels[2];

    cv::bilateralFilter(value, bilateral, 9, 75, 75);
    cv::GaussianBlur(bilateral, guassian, cv::Size(5, 5), 1.2);

    cv::Canny(guassian, canny, 30, 100);
    Edges = canny;

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
    cv::morphologyEx(canny, morphology, cv::MORPH_CLOSE, kernel);

    cv::dilate(morphology, processResult, cv::Mat(), cv::Point(-1, -1), dilationIterations);

    Dilation = processResult;

    return processResult;
}

const cv::Mat& FrameProcessing::getEdges() const {
    return Edges;
}

const cv::Mat& FrameProcessing::getDilated() const {
    return Dilation;
}