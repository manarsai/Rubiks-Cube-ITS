#include "Vision.h"
#include <algorithm>
#include <stdexcept>
#include "../core/domain/Cube.h"


// open the camera
Vision::Vision(int cameraIndex)
{
    cap.open(cameraIndex);
}

// free the camera when the object is destroyed
Vision::~Vision()
{
    if (cap.isOpened())
        cap.release();
}


cv::Mat Vision::getProcessedFrame()
{
    // if the camera fails, return empty im age
    if (!cap.isOpened())
        return cv::Mat();

    cap >> frame;
    if (frame.empty())
        return cv::Mat();

    // preprocessing
    cv::Mat edges = processor.process(frame);
    cv::Mat cleanEdges = processor.getEdges();

    // detect squares
    std::vector<std::vector<cv::Point>> squares =
        detector.detect(cleanEdges);

    lastContours = squares;

    // display setup
    cv::Mat display;
    frame.copyTo(display);

    // draw contours 
    cv::drawContours(display, squares, -1,
        cv::Scalar(0, 0, 255), 2);

    // colour classification
    std::vector<DetectedSquare> detected;

    // loop over each square
    for (const auto& square : squares)
    {
        cv::Rect r = cv::boundingRect(square);

        cv::Mat roi = classifier.extractROI(frame, r);

        if (roi.empty())
            continue;

        Colour colour = classifier.classify(roi);

        cv::Point center(
            r.x + r.width / 2,
            r.y + r.height / 2
        );

        detected.push_back({ center, colour });

        // label colour
        std::string text = classifier.toString(colour);

        cv::putText(display, text,
            cv::Point(r.x, r.y - 5),
            cv::FONT_HERSHEY_SIMPLEX,
            0.4,
            cv::Scalar(255, 255, 255),
            1);
    }

    // form the squares into a cube face
    if (detected.size() == 9)
    {
     
        detected = faceBuilder.sortFace(detected);

        // sort squares in order
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



// Output cube face
std::array<Colour, 9> Vision::getFaceColours()
{
    std::array<Colour, 9> face;

    if (lastDetectedFace.size() != 9)
        return face;

    for (int i = 0; i < 9; i++)
        face[i] = lastDetectedFace[i].colour;

    return face;
}