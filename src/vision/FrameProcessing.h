#pragma once
#include <opencv2/opencv.hpp>

class FrameProcessing {
public:
	cv::Mat process(const cv::Mat& frame);

	const cv::Mat& getEdges() const;
	const cv::Mat& getDilated() const;

private:
	cv::Mat Edges;
	cv::Mat Dilation;

	int dilationIterations = 1;

};