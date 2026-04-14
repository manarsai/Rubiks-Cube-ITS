#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../../vision/vision.h"

// -------------------------
// Cube state for one face
// -------------------------
struct CubeState
{
    CubeColour face[3][3];

    CubeState()
    {
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                face[r][c] = CubeColour::Unknown;
    }

    // Convert CubeColour to single character (W,R,O,Y,G,B)
    static char colorToChar(CubeColour c)
    {
        switch (c)
        {
        case CubeColour::White:  return 'W';
        case CubeColour::Red:    return 'R';
        case CubeColour::Orange: return 'O';
        case CubeColour::Yellow: return 'Y';
        case CubeColour::Green:  return 'G';
        case CubeColour::Blue:   return 'B';
        default:                 return 'X'; // unknown
        }
    }

    // Print the 3x3 face
    void printFace() const
    {
        for (int r = 0; r < 3; ++r)
        {
            for (int c = 0; c < 3; ++c)
                std::cout << colorToChar(face[r][c]);
            std::cout << std::endl;
        }
    }

    // Assign colors to 3x3 grid based on detected squares
    // squares: detected contours, colors: detected CubeColour aligned with squares
    void assignSquaresToFace(const std::vector<std::vector<cv::Point>>& squares,
        const std::vector<CubeColour>& colors)
    {
        if (squares.size() != colors.size()) return;

        // Compute bounding box covering the whole face
        int topY = INT_MAX, leftX = INT_MAX;
        int bottomY = 0, rightX = 0;
        std::vector<cv::Rect> rects;
        for (const auto& sq : squares)
        {
            cv::Rect r = cv::boundingRect(sq);
            rects.push_back(r);
            topY = std::min(topY, r.y);
            leftX = std::min(leftX, r.x);
            bottomY = std::max(bottomY, r.y + r.height);
            rightX = std::max(rightX, r.x + r.width);
        }

        float rowHeight = float(bottomY - topY) / 3.0f;
        float colWidth = float(rightX - leftX) / 3.0f;

        // Clear face
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                face[r][c] = CubeColour::Unknown;

        // Map each square to a row and column
        for (size_t i = 0; i < squares.size(); ++i)
        {
            cv::Rect r = rects[i];
            cv::Point center(r.x + r.width / 2, r.y + r.height / 2);

            int row = std::min(int((center.y - topY) / rowHeight), 2);
            int col = std::min(int((center.x - leftX) / colWidth), 2);

            // Flip vertically so top of image is row 0
            row = 2 - row;

            // Flip horizontally if needed (camera mirrored)
            // col = 2 - col;

            face[row][col] = colors[i];
        }
    }
};