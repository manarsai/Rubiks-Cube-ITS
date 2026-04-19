#include "DetectCubeSquares.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>

// ===============================
// Main Detection Pipeline
// ===============================

std::vector<std::vector<cv::Point>>
DetectCubeSquares::detect(const cv::Mat& edges)
{
    auto contours = detectContours(edges);
    auto approx = approximateContours(contours);

    std::vector<std::vector<cv::Point>> squares;

    for (const auto& c : approx)
    {
        if (isSquare(c))
            squares.push_back(c);
    }

    // Post-processing (order matters)
    squares = filterStableSquares(squares);
    squares = removeDuplicates(squares);

    return squares;
}

// ===============================
// Contour Detection
// ===============================

std::vector<std::vector<cv::Point>>
DetectCubeSquares::detectContours(const cv::Mat& edges)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> filtered;

    cv::findContours(edges,
        contours,
        cv::RETR_LIST,
        cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours)
    {
        double area = cv::contourArea(contour);

        if (area < 500)
            continue;

        filtered.push_back(contour);
    }

    return filtered;
}

// ===============================
// Contour Approximation
// ===============================

std::vector<std::vector<cv::Point>>
DetectCubeSquares::approximateContours(
    const std::vector<std::vector<cv::Point>>& contours)
{
    std::vector<std::vector<cv::Point>> approxContours;

    for (const auto& contour : contours)
    {
        std::vector<cv::Point> approx;

        double epsilon =
            approxEpsilonFactor *
            cv::arcLength(contour, true);

        cv::approxPolyDP(contour,
            approx,
            epsilon,
            true);

        approxContours.push_back(approx);
    }

    return approxContours;
}

// ===============================
// Square Validation
// ===============================

bool DetectCubeSquares::isSquare(const std::vector<cv::Point>& contour)
{
    if (contour.size() < 4 || contour.size() > 6)
        return false;

    double area = std::abs(cv::contourArea(contour));
    if (area < 300)
        return false;

    auto distance = [](cv::Point a, cv::Point b)
        {
            return std::sqrt(
                (a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y));
        };

    double d1 = distance(contour[0], contour[1]);
    double d2 = distance(contour[1], contour[2]);
    double d3 = distance(contour[2], contour[3]);
    double d4 = distance(contour[3], contour[0]);

    double maxSide = std::max({ d1, d2, d3, d4 });
    double minSide = std::min({ d1, d2, d3, d4 });

    if (minSide / maxSide < 0.45)
        return false;

    double aspect = maxSide / minSide;
    if (aspect > 2.0)
        return false;

    double rectArea = cv::boundingRect(contour).area();
    double areaRatio = area / rectArea;

    if (areaRatio < 0.6)
        return false;

    auto angle = [](cv::Point a, cv::Point b, cv::Point c)
        {
            cv::Point ab = b - a;
            cv::Point bc = c - b;

            double dot = ab.x * bc.x + ab.y * bc.y;
            double mag = std::sqrt(ab.x * ab.x + ab.y * ab.y) *
                std::sqrt(bc.x * bc.x + bc.y * bc.y);

            if (mag == 0) return 0.0;

            return std::acos(dot / mag) * 180.0 / CV_PI;
        };

    double a1 = angle(contour[0], contour[1], contour[2]);
    double a2 = angle(contour[1], contour[2], contour[3]);
    double a3 = angle(contour[2], contour[3], contour[0]);
    double a4 = angle(contour[3], contour[0], contour[1]);

    if (std::abs(a1 - 90) > 20) return false;
    if (std::abs(a2 - 90) > 20) return false;
    if (std::abs(a3 - 90) > 20) return false;
    if (std::abs(a4 - 90) > 20) return false;

    return true;
}

// ===============================
// Temporal Stability
// ===============================

std::vector<std::vector<cv::Point>>
DetectCubeSquares::filterStableSquares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> stable;

    for (const auto& sq : squares)
    {
        cv::Rect r = cv::boundingRect(sq);

        bool matched = false;

        for (const auto& oldR : lastSquares)
        {
            cv::Rect inter = r & oldR;

            double overlap =
                (double)inter.area() /
                std::min(r.area(), oldR.area());

            if (overlap > 0.5)
            {
                matched = true;
                break;
            }
        }

        if (matched || r.area() > 1000)
            stable.push_back(sq);
    }

    // update memory
    lastSquares.clear();
    for (const auto& sq : stable)
        lastSquares.push_back(cv::boundingRect(sq));

    return stable;
}

// ===============================
// Duplicate Removal
// ===============================

std::vector<std::vector<cv::Point>>
DetectCubeSquares::removeDuplicates(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> unique;

    for (const auto& sq : squares)
    {
        cv::Rect r1 = cv::boundingRect(sq);
        bool duplicate = false;

        for (const auto& usq : unique)
        {
            cv::Rect r2 = cv::boundingRect(usq);

            cv::Rect inter = r1 & r2;

            double overlap =
                (double)inter.area() /
                std::min(r1.area(), r2.area());

            if (overlap > 0.6)
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate)
            unique.push_back(sq);
    }

    return unique;
}