//#include "CubeModel.h"
//
//CubeModel& CubeModel::get()
//{
//    static CubeModel instance;
//    return instance;
//}
//
//void CubeModel::setFace(int faceIndex, const std::array<Colour, 9>& face)
//{
//    if (faceIndex < 0 || faceIndex >= 6)
//        return;
//
//    faces[faceIndex] = face;
//    filled[faceIndex] = true;
//}
//
//const std::array<std::array<Colour, 9>, 6>& CubeModel::getFaces() const
//{
//    return faces;
//}
//
//void CubeModel::reset()
//{
//    for (int i = 0; i < 6; i++)
//    {
//        filled[i] = false;
//        for (int j = 0; j < 9; j++)
//            faces[i][j] = Colour::UNKNOWN;
//    }
//}
//
//bool CubeModel::isEmpty() const
//{
//    for (bool f : filled)
//        if (f) return false;
//
//    return true;
//}