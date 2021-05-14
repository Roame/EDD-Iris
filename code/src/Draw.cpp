#include "Draw.h"


Matrix Draw::drawLine(Matrix &image, std::vector<float> line){
    std::vector<int> dims = image.getDimensions();
    std::vector<float> data = image.getData();
    if(abs(line[0]) < 1){
        for(int t = 0; t < dims[1]; t++){
            int y = (int)(line[0]*t+line[1]);
            if(y > 239 || y < 0) continue;
            int index = 3*(t+y*dims[1]);
            data[index] = 66;
            data[index+1] = 135;
            data[index+2] = 245;
        }
    } else {
        for(int t = 0; t < dims[0]; t++){
            int x = (int)((t-line[1])/line[0]);
            if(x > 359 || x < 0) continue;
            int index = (int) 3*(x+t*dims[1]);
            data[index] = 66;
            data[index+1] = 135;
            data[index+2] = 245;
        }
    }
    return Matrix(dims, data);
}


Matrix Draw::drawPoint(Matrix &image, int x, int y){
    std::vector<int> dims = image.getDimensions();
    std::vector<float> data = image.getData();
    int sideL = 20;
    for(int i = 0; i< sideL; i++){
        for(int j = 0; j < sideL; j++){
            int drawX = x+i-sideL/2;
            int drawY = y+j-sideL/2;
            int index = 3*(drawX+drawY*dims[1]);
            data[index] = 66;
            data[index+1] = 135;
            data[index+2] = 245;
        }
    }
    return Matrix(dims, data);
}

Matrix Draw::drawBox(Matrix &image, int xLow, int xHigh, int yLow, int yHigh){
    std::vector<int> dims = image.getDimensions();
    std::vector<float> data = image.getData();
    int r = 66, g = 135, b = 245;
    int width = xHigh-xLow;
    for(int j = yLow; j <= yHigh; j++){
        if(j == yLow || j == yHigh){
            for(int i = xLow; i <= xHigh; i++){
                int index = 3*(i+j*dims[1]);
                data[index] = r;
                data[index+1] = g;
                data[index+2] = b;
            }
        } else {
            int index = 3*(xLow+j*dims[1]);
            data[index] = r;
            data[index+1] = g;
            data[index+2] = b;
            index = 3*(xHigh+j*dims[1]);
            data[index] = r;
            data[index+1] = g;
            data[index+2] = b;
        }
    }
    return Matrix(dims, data);
}
