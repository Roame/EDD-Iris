#include "MatrixOps.h"



Matrix MatrixOps::applyMask(Matrix &image, Matrix &mask){
    std::vector<int> outDims = image.getDimensions();
    std::vector<float> outData = image.getData();
    std::vector<float> maskData = mask.getData();
    for(int i = 0; i < maskData.size(); i++){
        outData[3*i] = outData[3*i] * maskData[i];
        outData[3*i+1] = outData[3*i+1] * maskData[i];
        outData[3*i+2] = outData[3*i+2] * maskData[i];
    }
    return Matrix(outDims, outData);
}

// It was at this point that I realized that the image data
// was organized along the x-axis first, not the y
// TODO: fix incoming image data and then all functions that work with image data
Matrix MatrixOps::edgeDetect(Matrix &rgbImage){
    std::vector<int> imageDims = rgbImage.getDimensions();
    std::vector<float> imageData = rgbImage.getData();

    std::vector<int> outDims{imageDims[0], imageDims[1]};
    std::vector<float> outData(imageDims[0] * imageDims[1]);
    float pRVal;
    float pGVal;
    float pBVal;
    bool edge = false;
    float threshold = 30;

    // Vertical iteration
    for(int i = 0; i < outDims[1]; i++){
        pRVal = imageData[3*i];
        pGVal = imageData[3*i+1];
        pBVal = imageData[3*i+2];
        for(int j = 1; j < outDims[0]; j++){
            int index = 3*(i+j*outDims[1]);
            edge = abs(pRVal-imageData[index]) > threshold ||
                    abs(pGVal-imageData[index+1]) > threshold ||
                    abs(pBVal-imageData[index+2]) > threshold;
            outData[index/3] = edge ? 1 : 0;
            pRVal = imageData[index];
            pGVal = imageData[index+1];
            pBVal = imageData[index+2];
        }
    }

    // Horizontal iteration
    for(int j = 0; j < outDims[0]; j++){
        pRVal = imageData[3*j*outDims[1]];
        pGVal = imageData[3*j*outDims[1]+1];
        pBVal = imageData[3*j*outDims[1]+2];
        for(int i = 1; i < outDims[1]; i++){
            int index = 3*(i+j*outDims[1]);
            edge = abs(pRVal-imageData[index]) > threshold ||
                    abs(pGVal-imageData[index+1]) > threshold ||
                    abs(pBVal-imageData[index+2]) > threshold;
            outData[index/3] = edge || outData[index/3] == 1 ? 1 : 0;
            pRVal = imageData[index];
            pGVal = imageData[index+1];
            pBVal = imageData[index+2];
        }
    }
    return Matrix(outDims, outData);
}

Matrix MatrixOps::downSample(Matrix &image){
    int stride = 2;
    std::vector<int> dims(image.getDimensions());
    std::vector<float> data(image.getData());

    std::vector<int> outDims{dims[0]/stride, dims[1]/stride, 3};
    std::vector<float> outData(outDims[0]*outDims[1]*3);
    for(int i = 0; i < outDims[1]; i++){
        for(int j = 0; j < outDims[0]; j++){
            int outIndex = 3*(i+j*outDims[1]);
            int index = stride*3*(i+j*dims[1]);
 
            outData[outIndex] = data[index];
            outData[outIndex+1] = data[index+1];
            outData[outIndex+2] = data[index+2];
        }
    }
    return Matrix(outDims, outData);
}