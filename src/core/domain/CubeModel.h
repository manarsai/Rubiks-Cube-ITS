//#pragma once
//
//#include <array>
//#include "Cube.h"
//
//class CubeModel
//{
//public:
//    static CubeModel& get();   // global access (simple ITS approach)
//
//    void setFace(int faceIndex, const std::array<Colour, 9>& face);
//    const std::array<std::array<Colour, 9>, 6>& getFaces() const;
//
//    void reset();
//    bool isEmpty() const;
//
//private:
//    CubeModel() = default;
//
//    std::array<std::array<Colour, 9>, 6> faces;
//    std::array<bool, 6> filled = { false, false, false, false, false, false };
//};