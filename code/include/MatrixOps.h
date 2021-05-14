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
    static Matrix downSample(Matrix &image);
    static Matrix cropImage(Matrix &image, int x1, int y1, int x2, int y2);
};