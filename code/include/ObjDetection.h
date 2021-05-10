#pragma once

#include <vector>

#include "Matrix.h"

class ObjDetection
{
private:
public:
    ObjDetection();
    ~ObjDetection();

    std::vector<float> getLines(Matrix &binaryImg, std::vector<float> knownLines);
};