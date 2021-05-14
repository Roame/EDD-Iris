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
    static std::vector<int> getRectPoints(std::vector<float> lines);
};