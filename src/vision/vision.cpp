#include "Vision.h"
#include <algorithm>
#include <stdexcept>
#include "../core/domain/Cube.h"


// open the camera

Vision::Vision(int cameraIndex)
{
    cap.open(cameraIndex);
}

// when the object is destroyed, close the camera
Vision::~Vision()
{
    if (cap.isOpened())
        cap.release();
}

// main processing pipeline

cv::Mat Vision::getProcessedFrame()
{
    // check camera
    if (!cap.isOpened())
        return cv::Mat();

    // grab a frame from the camera
    // if failed, return an empty image
    cap >> lastFrame;
    if (lastFrame.empty())
        return cv::Mat();

    // preprocessing
    cv::Mat edges = preprocessFrame(lastFrame);

    // detect contours
    std::vector<std::vector<cv::Point>> rawContours = detectContours(edges);

    // approximate contours
    std::vector<std::vector<cv::Point>> approxContours = approximateContours(rawContours);

    // filter for squares
    std::vector<std::vector<cv::Point>> squares;
    for (const auto& contour : approxContours)
    {
        if (isSquare(contour))
            squares.push_back(contour);
    }

    std::vector<std::vector<cv::Point>> stableSquares;

    for (const auto& sq : squares)
    {
        cv::Rect r = cv::boundingRect(sq);

        bool matched = false;
        for (const auto& oldR : lastSquares)
        {
            cv::Rect inter = r & oldR;
            double overlap = (double)inter.area() / std::min(r.area(), oldR.area());
            if (overlap > 0.5)
            {
                matched = true;
                break;
            }
        }

        // Keep squares that match previous frame or are large enough
        if (matched || r.area() > 1000)
            stableSquares.push_back(sq);
    }

    // Save current squares for next frame
    lastSquares.clear();
    for (const auto& sq : stableSquares)
        lastSquares.push_back(cv::boundingRect(sq));

    // Replace squares with the stable version
    squares = stableSquares;





    std::vector<std::vector<cv::Point>> uniqueSquares;

    for (const auto& sq : squares)
    {
        cv::Rect r1 = cv::boundingRect(sq);
        bool duplicate = false;

        for (const auto& usq : uniqueSquares)
        {
            cv::Rect r2 = cv::boundingRect(usq);

            // Compute intersection
            cv::Rect inter = r1 & r2;
            double overlap = (double)inter.area() / std::min(r1.area(), r2.area());

            if (overlap > 0.6) // threshold
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate)
            uniqueSquares.push_back(sq);
    }

    // Replace squares with filtered version
    squares = uniqueSquares;

    lastContours = squares;

    // prepare display frame
    cv::Mat display;
    lastFrame.copyTo(display);

    // debug contours
    //cv::drawContours(display, rawContours, -1, cv::Scalar(255, 0, 0), 1);   // raw (blue)
    //cv::drawContours(display, approxContours, -1, cv::Scalar(0, 255, 0), 2); // approx (green)
    cv::drawContours(display, squares, -1, cv::Scalar(0, 0, 255), 2);        // final squares (red)

    // hsv color detection
    std::vector<DetectedSquare> detected;

    for (const auto& square : squares)
    {
        cv::Rect r = cv::boundingRect(square);

        int padding = static_cast<int>(r.width * 0.2);
        r.x += padding;
        r.y += padding;
        r.width -= 2 * padding;
        r.height -= 2 * padding;

        r &= cv::Rect(0, 0, lastFrame.cols, lastFrame.rows);
        if (r.width <= 0 || r.height <= 0)
            continue;

        cv::Mat roi = lastFrame(r);

        CubeColour cubeColor = classifySquareHSV(roi);

        // ✅ NEW: store center + colour
        cv::Point center(r.x + r.width / 2, r.y + r.height / 2);
        detected.push_back({ center, cubeColor });

        // keep your debug text
        std::string text = cubeColourToString(cubeColor);
        cv::putText(display, text, cv::Point(r.x, r.y - 5),
            cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
    }

    if (detected.size() == 9)
    {
        std::sort(detected.begin(), detected.end(),
            [](const DetectedSquare& a, const DetectedSquare& b) {
                if (abs(a.center.y - b.center.y) > 25)
                    return a.center.y < b.center.y;
                return a.center.x < b.center.x;
            });

        for (int i = 0; i < 9; i++)
        {
            cv::putText(display,
                std::to_string(i),
                detected[i].center,
                cv::FONT_HERSHEY_SIMPLEX,
                1.0,
                cv::Scalar(0, 255, 255),
                2);
        }

        lastDetectedFace = detected;
    }

    return display;
}

// Preprocessing


cv::Mat Vision::preprocessFrame(const cv::Mat & frame)
{
    cv::Mat gray, enhanced, smooth, edges, closedEdges, blah;

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    cv::Mat value = channels[2]; 

    cv::bilateralFilter(value, blah, 9, 75, 75);
    cv::GaussianBlur(blah, smooth, cv::Size(5, 5), 1.2);
    //cv::GaussianBlur(value, smooth, cv::Size(5, 5), 1.2);


    // edge detection
    cv::Canny(smooth, edges, 30, 100);
    lastEdges = edges;

    //morphological closing
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
    cv::morphologyEx(edges, closedEdges, cv::MORPH_CLOSE, kernel);

    // dilation
    cv::dilate(closedEdges, closedEdges, cv::Mat(), cv::Point(-1, -1), dilationIterations);
  
    lastDilated = closedEdges;
    return closedEdges;
}


// Contour Detection
std::vector<std::vector<cv::Point>>
Vision::detectContours(const cv::Mat& edges)
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

// Contour Approximation
std::vector<std::vector<cv::Point>>
Vision::approximateContours(
    const std::vector<std::vector<cv::Point>>&contours)
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

bool Vision::isSquare(const std::vector<cv::Point>&contour)
{
    // Allow 4–6 points
    if (contour.size() < 4 || contour.size() > 6)
        return false;

    // Reject very small noise
    double area = std::abs(cv::contourArea(contour));
    if (area < 300)
        return false;

    // Side lengths
    auto distance = [](cv::Point a, cv::Point b)
        {
            return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        };

    double d1 = distance(contour[0], contour[1]);
    double d2 = distance(contour[1], contour[2]);
    double d3 = distance(contour[2], contour[3]);
    double d4 = distance(contour[3], contour[0]);

    double maxSide = std::max({ d1,d2,d3,d4 });
    double minSide = std::min({ d1,d2,d3,d4 });

    if (minSide / maxSide < 0.45) // relaxed ratio
        return false;

    // Aspect ratio check
    double aspect = maxSide / minSide;
    if (aspect > 2.0)
        return false;

    // Area ratio (filter small nested contours inside big squares)
    double rectArea = cv::boundingRect(contour).area();
    double areaRatio = area / rectArea;
    if (areaRatio < 0.6)
        return false;

    // Angle check helper
    auto angle = [](cv::Point a, cv::Point b, cv::Point c)
        {
            cv::Point ab = b - a;
            cv::Point bc = c - b;
            double dot = ab.x * bc.x + ab.y * bc.y;
            double mag = std::sqrt(ab.x * ab.x + ab.y * ab.y) * std::sqrt(bc.x * bc.x + bc.y * bc.y);
            if (mag == 0) return 0.0;
            return std::acos(dot / mag) * 180.0 / CV_PI;
        };

    double a1 = angle(contour[0], contour[1], contour[2]);
    double a2 = angle(contour[1], contour[2], contour[3]);
    double a3 = angle(contour[2], contour[3], contour[0]);
    double a4 = angle(contour[3], contour[0], contour[1]);

    // Relaxed angle tolerance
    if (std::abs(a1 - 90) > 20) return false;
    if (std::abs(a2 - 90) > 20) return false;
    if (std::abs(a3 - 90) > 20) return false;
    if (std::abs(a4 - 90) > 20) return false;

    return true;
}

CubeColour Vision::classifySquareHSV(const cv::Mat & roi)
{
    if (roi.empty())
        return CubeColour::Unknown;

    // 1️⃣ Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);

    // 2️⃣ Collect H, S, V channels
    std::vector<uchar> hVals, sVals, vVals;
    for (int i = 0; i < hsv.rows; ++i) {
        for (int j = 0; j < hsv.cols; ++j) {
            cv::Vec3b px = hsv.at<cv::Vec3b>(i, j);
            hVals.push_back(px[0]);
            sVals.push_back(px[1]);
            vVals.push_back(px[2]);
        }
    }

    // 3️⃣ Compute medians
    auto median = [](std::vector<uchar>& vals) -> int {
        std::nth_element(vals.begin(), vals.begin() + vals.size() / 2, vals.end());
        return vals[vals.size() / 2];
        };

    int h = median(hVals);
    int s = median(sVals);
    int v = median(vVals);

    // 4️⃣ Adaptive brightness normalization
    double targetV = 150.0;
    double scaleV = targetV / (v + 1e-5);
    v = std::min(int(v * scaleV), 255);
    if (s < 50) s = std::min(int(s * 1.5), 255); // optional saturation boost

    // 5️⃣ Robust color classification
    if (v < 50)
        return CubeColour::Unknown;                // black or too dark
    else if (s < 90 && v > 120)
        return CubeColour::White;                  // bright, low saturation = white
    else if ((h >= 0 && h <= 10) || (h >= 160 && h <= 179))
        return CubeColour::Red;
    else if (h > 10 && h <= 25)
        return CubeColour::Orange;
    else if (h > 25 && h <= 35)
        return CubeColour::Yellow;
    else if (h > 35 && h <= 85)
        return CubeColour::Green;
    else if (h > 85 && h <= 130)
        return CubeColour::Blue;
    else
        return CubeColour::Unknown;
}

std::string Vision::cubeColourToString(CubeColour color)
{
    switch (color)
    {
    case CubeColour::White:  return "WHITE";
    case CubeColour::Red:    return "RED";
    case CubeColour::Orange: return "ORANGE";
    case CubeColour::Yellow: return "YELLOW";
    case CubeColour::Green:  return "GREEN";
    case CubeColour::Blue:   return "BLUE";
    default:                 return "UNKNOWN";
    }
}

Color Vision::convertToCubeColor(CubeColour c)
{
    switch (c) {
    case CubeColour::White: return Color::WHITE;
    case CubeColour::Red: return Color::RED;
    case CubeColour::Blue: return Color::BLUE;
    case CubeColour::Yellow: return Color::YELLOW;
    case CubeColour::Orange: return Color::ORANGE;
    case CubeColour::Green: return Color::GREEN;
    default: return Color::WHITE;
    }
}

std::array<Color, 9> Vision::getFaceColors()
{
    std::array<Color, 9> face;

    if (lastDetectedFace.size() != 9)
        throw std::runtime_error("Face not detected properly");

    for (int i = 0; i < 9; i++)
        face[i] = convertToCubeColor(lastDetectedFace[i].colour);

    return face;
}