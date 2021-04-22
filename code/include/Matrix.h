#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string.h>

class Matrix {
private:
    std::vector<float> data;
    std::vector<int> dims;
public:
    Matrix();
    Matrix(std::vector<int> &dimensions, std::vector<float> &data);
    ~Matrix();
    const char* toString();
    std::vector<float> getData();
};
