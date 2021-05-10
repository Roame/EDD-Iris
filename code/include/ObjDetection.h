#pragma once

#include <iostream>
#include <vector>

#include "Matrix.h"

class ObjDetection
{
private:
public:
    ObjDetection();
    ~ObjDetection();

    static std::vector<float> getLines(Matrix &binaryImg, std::vector<float> knownLines);
};