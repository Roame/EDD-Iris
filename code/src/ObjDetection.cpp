#include "ObjDetection.h"

// float colors[] = {255 0};

bool checkProximity(float x, float y, std::vector<float> &lines){
    // Returns true if the point is close to line
    float threshold = 5; // How many pixels away is considered close enough to be a part of the line
    for(int k = 0; k < lines.size()/2; k++){
        float a = lines[k], b = lines[k+1];
        float intersectX = (a/(a*a + 1)) * (y + (1.0/a)*x - b);
        float intersectY = a*intersectX + b;
        float dist = sqrt(pow(intersectX-x, 2)+ pow(intersectY-y, 2));
        if(dist <= threshold){
            return true;
        }
    }
    return false;
}

std::vector<float> constructLine(Matrix &binImg, float x, float y){
    bool deadEnd = false;
    while(!deadEnd){
        // TODO: fill this in please
    }
}

std::vector<float> ObjDetection::getLines(Matrix &binaryImg, std::vector<float> knownLines){
    std::vector<int> dims = binaryImg.getDimensions();
    std::vector<float> data = binaryImg.getData();

    for(int i = 0; i < dims[1]; i++){
        for(int j = 0; j < dims[0]; j++){
            int index = (j*dims[1]+i);
            if(data[index] == 1 && !checkProximity(i, j, knownLines)){
                // Empty atm
            }
        }
    }
}