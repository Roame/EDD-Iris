#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <cmath>

class Matrix {
private:
    std::vector<float> data;
    std::vector<int> dims;
public:
    Matrix();
    Matrix(std::vector<int> &dimensions, std::vector<float> &data);
    ~Matrix();
    const char* toString();
    std::vector<int> getDimensions();
    std::vector<float> getData();

    static Matrix maskFromDeltas(Matrix &deltas, float threshold);
    static Matrix hadamardProduct(Matrix &m1, Matrix &m2);

    Matrix operator+ (Matrix const &obj);
    Matrix operator* (float scalar);
};
