#include "Draw.h"


Matrix Draw::drawLine(Matrix &image, std::vector<float> line){
    std::vector<int> dims = image.getDimensions();
    std::vector<float> data = image.getData();

    for(int x = 0; x < dims[1]; x++){
        int y = (int)(line[0]*x+line[1]);
        if(y > 239 || y < 0) continue;
        int index = 3*(x+y*dims[1]);
        data[index] = 66;
        data[index+1] = 135;
        data[index+2] = 245;
    }
    return Matrix(dims, data);
}