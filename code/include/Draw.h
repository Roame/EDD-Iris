#pragma once

#include <iostream>
#include <vector>

#include "Matrix.h"

class Draw
{
private:
public:
    Draw();
    ~Draw();
    static Matrix drawLine(Matrix &image, std::vector<float> line);
};