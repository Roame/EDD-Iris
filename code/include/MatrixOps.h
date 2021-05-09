#pragma once

#include "Matrix.h"

class MatrixOps
{
private:
public:
    MatrixOps(){}
    ~MatrixOps(){}
    static Matrix applyMask(Matrix &image, Matrix &mask);
    static Matrix edgeDetect(Matrix &rgbImage);
};