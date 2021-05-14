#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Matrix.h"

class ObjDetection
{
private:
public:
    ObjDetection();
    ~ObjDetection();

    static std::vector<float> getLines(Matrix &binaryImg, std::vector<float> knownLines);
    static std::vector<float> getRectLines(int filterIndex, std::vector<float> lines);
    static std::vector<int> getXDistribution(Matrix &edges);
    static std::vector<int> getYDistribution(Matrix &edges);
    static std::vector<int> getCorners(std::vector<int> &distX, std::vector<int> &distY, Matrix &edges);
    static std::vector<int> getBox(std::vector<int> &distX, std::vector<int> &distY);
};