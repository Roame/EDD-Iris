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
    static Matrix drawPoint(Matrix &image, int x, int y);
    static Matrix drawBox(Matrix &image, int xLow, int xHigh, int yLow, int yHigh);
};