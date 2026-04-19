#include "ColourClassification.h"
#include "../core/domain/Cube.h"

Colour ColourClassification::classify(const cv::Mat& roi)
{
    if (roi.empty())
        return Colour:: UNKNOWN;

    cv::Mat hsv;
    cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);

    std::vector<uchar> hVals, sVals, vVals;

    for (int i = 0; i < hsv.rows; ++i)
    {
        for (int j = 0; j < hsv.cols; ++j)
        {
            cv::Vec3b px = hsv.at<cv::Vec3b>(i, j);
            hVals.push_back(px[0]);
            sVals.push_back(px[1]);
            vVals.push_back(px[2]);
        }
    }

    int h = median(hVals);
    int s = median(sVals);
    int v = median(vVals);

    double targetV = 150.0;
    double scaleV = targetV / (v + 1e-5);
    v = std::min(int(v * scaleV), 255);

    if (s < 50) s = std::min(int(s * 1.5), 255);

    if (v < 50)
        return Colour::UNKNOWN;

    else if (s < 90 && v > 120)
        return Colour::WHITE;

    else if ((h >= 0 && h <= 10) || (h >= 160 && h <= 179))
        return Colour::RED;

    else if (h > 10 && h <= 25)
        return Colour::ORANGE;

    else if (h > 25 && h <= 35)
        return Colour::YELLOW;

    else if (h > 35 && h <= 85)
        return Colour::GREEN;

    else if (h > 85 && h <= 130)
        return Colour::BLUE;

    return Colour::UNKNOWN;
}

int ColourClassification::median(std::vector<uchar>& vals)
{
    std::nth_element(vals.begin(),
        vals.begin() + vals.size() / 2,
        vals.end());
    return vals[vals.size() / 2];
}

std::string ColourClassification::toString(Colour colour)
{
    switch (colour)
    {
    case Colour::WHITE:  return "WHITE";
    case Colour::RED:    return "RED";
    case Colour::ORANGE: return "ORANGE";
    case Colour::YELLOW: return "YELLOW";
    case Colour::GREEN:  return "GREEN";
    case Colour::BLUE:   return "BLUE";
    default:                 return "UNKNOWN";
    }
}

cv::Mat ColourClassification::extractROI(const cv::Mat& frame, const cv::Rect& r)
{
    cv::Rect region = r;

    int padding = static_cast<int>(region.width * 0.2);

    region.x += padding;
    region.y += padding;
    region.width -= 2 * padding;
    region.height -= 2 * padding;

    region &= cv::Rect(0, 0, frame.cols, frame.rows);

    if (region.width <= 0 || region.height <= 0)
        return cv::Mat();

    return frame(region);
}