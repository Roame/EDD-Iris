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

std::vector<float> constructLine(Matrix &binImg, int x, int y){
    std::vector<int> dims = binImg.getDimensions();
    std::vector<float> data = binImg.getData();
    bool deadEnd = false, firstPoint = true, success = false;
    int startX = x, startY = y;
    int count = 1, minCount = 15; // Mininum required to be considered a line
    int circle[] = {    -1, 2, 0, 2, 1, 2,
                         2, 1, 2, 0, 2,-1,
                         1,-2, 0,-2,-1,-2,
                        -2,-1,-2, 0,-2, 1 }; //Quick outline of a circle
    int plus[] = {   0,  0,
                     0,  1,
                     1,  0,
                     0, -1,
                    -1,  0};
    while(!deadEnd){
        success = false; // Reset for each point
        if(firstPoint){
            for(int i = 0; i < sizeof(circle)/sizeof(int)/2; i++){
                int testX = startX + circle[2*i];
                int testY = startY + circle[2*i+1];
                int index = testX + testY*binImg.getDimensions()[1];
                if(data[index] == 1){
                    x = testX;
                    y = testY;
                    count++;
                    success = true;
                    break;
                }
            }
            if(!success) return std::vector<float>();
            firstPoint = false;
        } else {
            int testX, testY;
            if(x-startX == 0){
                testX = startX;
            } else {
                testX = (int)((float)(x-startX)/(float)abs(x-startX)*count+startX);
            }
            if(y-startY == 0){
                testY = startY;
            } else {
                testY = (int)((float)(y-startY)/(float)abs(x-startX)*count+startY);
            }

            for(int i = 0; i < sizeof(plus)/sizeof(int)/2; i++){
                int plusX = testX + plus[2*i];
                int plusY = testY + plus[2*i+1];
                if(plusX>dims[1] || plusX<0 || plusY>dims[0] || plusY<0) continue;
                int index = plusX + plusY*binImg.getDimensions()[1];
                if(data[index] == 1){
                    x = plusX;
                    y = plusY;
                    count++;
                    success = true;
                    break;
                }
            }
            if(!success) deadEnd = true;
        }
    }

    if(count < minCount) return std::vector<float>();

    float slope = x-startX != 0 ? (float)(y-startY)/(x-startX) : 1000.0; // Aproximating a vertical line at worst
    float yIntercept = (float) startY-slope*startX;
    return std::vector<float>{slope, yIntercept};
}

std::vector<float> ObjDetection::getLines(Matrix &binaryImg, std::vector<float> knownLines){
    std::vector<int> dims = binaryImg.getDimensions();
    std::vector<float> data = binaryImg.getData();

    for(int i = 0; i < dims[1]; i++){
        for(int j = 0; j < dims[0]; j++){
            int index = (j*dims[1]+i);
            if(data[index] == 1 && !checkProximity(i, j, knownLines)){
                std::vector<float> line = constructLine(binaryImg, i, j);
                knownLines.insert(knownLines.begin(), line.begin(), line.end());
            }
        }
    }
    return knownLines;
}