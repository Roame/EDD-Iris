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