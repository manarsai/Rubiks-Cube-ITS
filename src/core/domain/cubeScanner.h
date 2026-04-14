//#pragma once
//#include <vector>
//#include <map>
//#include <deque>
//#include <opencv2/opencv.hpp>
//#include "../../vision/vision.h"
//#include "cubestate.h"
//
//// -------------------------
//// CubeScanner: scans full cube faces
//// -------------------------
//struct CubeScanner
//{
//    Vision* vision; // pointer to your Vision object
//    std::map<CubeColour, CubeState> cubeFaces; // store face states keyed by center color
//    std::vector<cv::Rect> lastSquares;         // for stabilizing positions
//    std::map<int, std::deque<CubeColour>> colorHistory; // per square, last N colors
//
//    CubeScanner(Vision* v) : vision(v) {}
//
//    // Scan one face, return CubeState
//    CubeState scanFace()
//    {
//        CubeState faceState;
//        cv::Mat frame = vision->getProcessedFrame(); // get processed frame
//
//        const auto& squares = vision->getLastContours(); // stabilized contours
//        std::vector<CubeColour> colors;
//
//        for (size_t i = 0; i < squares.size(); ++i)
//        {
//            cv::Rect r = cv::boundingRect(squares[i]);
//            cv::Mat roi = frame(r);
//
//            // Dark red boost
//            cv::Mat hsv;
//            cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);
//
//            // median hue
//            std::vector<uchar> hVals;
//            for (int y = 0; y < hsv.rows; ++y)
//                for (int x = 0; x < hsv.cols; ++x)
//                    hVals.push_back(hsv.at<cv::Vec3b>(y, x)[0]);
//
//            std::nth_element(hVals.begin(), hVals.begin() + hVals.size() / 2, hVals.end());
//            int medianH = hVals[hVals.size() / 2];
//
//            if ((medianH >= 0 && medianH <= 15) || (medianH >= 150 && medianH <= 179))
//            {
//                cv::Mat channels[3];
//                cv::split(hsv, channels);
//                channels[2] = cv::min(channels[2] * 1.8, 255); // brighten red
//                cv::merge(channels, 3, hsv);
//            }
//
//            CubeColour c = vision->classifySquareHSV(hsv);
//
//            // Stabilize color over last 5 frames
//            auto& hist = colorHistory[i];
//            hist.push_back(c);
//            if (hist.size() > 5) hist.pop_front();
//
//            std::map<CubeColour, int> freq;
//            for (CubeColour fc : hist) freq[fc]++;
//            CubeColour stable = CubeColour::Unknown;
//            int maxF = 0;
//            for (auto& p : freq)
//            {
//                if (p.second > maxF) { maxF = p.second; stable = p.first; }
//            }
//
//            colors.push_back(stable);
//        }
//
//        // Assign squares to 3x3 grid
//        faceState.assignSquaresToFace(squares, colors);
//
//        // Save current squares for position stabilization next frame
//        lastSquares.clear();
//        for (const auto& sq : squares)
//            lastSquares.push_back(cv::boundingRect(sq));
//
//        return faceState;
//    }
//
//    // Scan multiple faces in order
//    //void scanCubeFaces(int numFaces = 6)
//    {    {
//        cubeFaces.clear();
//        for (int f = 0; f < numFaces; ++f)
//        {
//            CubeState face = scanFace();
//            CubeColour center = face.face[1][1]; // center square determines face
//            cubeFaces[center] = face;
//        }
//    }
//
//    // Print the full cube state
//    void printCube()
//    {
//        for (auto& p : cubeFaces)
//        {
//            char faceLabel = CubeState::colorToChar(p.first);
//            std::cout << "Face " << faceLabel << ":\n";
//            p.second.printFace();
//            std::cout << "\n";
//        }
//    }
//};