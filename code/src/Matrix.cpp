#include "Matrix.h"

using namespace std;

Matrix::Matrix(){}

Matrix::Matrix(vector<int> &dimensions, vector<float> &data){
    this->data = data;
    this->dims = dimensions;

}

Matrix::~Matrix(){
    
}


// TODO: Currently experiencing memory issues when printing large arrays
const char* Matrix::toString(){
    // static stringstream sstm;
    // sstm << fixed << setprecision(2);
    // vector<float>::iterator it;
    // int counter = 0 ;
    // for(it = data.begin(); it != data.end(); it++){
    //     sstm << *it << " ";
    //     if(counter % (dims[0]*dims[1]) == dims[0]*dims[1]-1){
    //         sstm << "\n";
    //     } else if (counter % dims[1] == dims[1]-1){
    //         sstm << "   ";
    //     }
    //     counter++;
    // }
    // return sstm.str().c_str();
    
    // sstm << data[0] << " " << data[1] << " " << data[2];
    // return sstm.str().c_str();
    return NULL;
}

vector<int> Matrix::getDimensions(){
    return dims;
}

vector<float> Matrix::getData(){
    return data;
}

Matrix Matrix::maskFromDeltas(Matrix &deltas, float threshold){
    vector<int> outDims(deltas.dims);
    vector<float> outData(deltas.data);
    for(int i = 0; i < (int)(outData.size()/3); i++){
        float avgDelta = (  abs(outData[3*i])+
                            abs(outData[3*i+1])+
                            abs(outData[3*i+2]))/3;
        if(avgDelta<threshold) {
            outData[3*i] = 0;
            outData[3*i+1] = 0;
            outData[3*i+2] = 0;
        } else {
            outData[3*i] = 1;
            outData[3*i+1] = 1;
            outData[3*i+2] = 1;
        }
    }
    return Matrix(outDims, outData);
}

Matrix Matrix::hadamardProduct(Matrix &m1, Matrix &m2) {
    if(m1.dims != m2.dims) throw "Matrices don't have matching dimensions";
    vector<int> outDims(m1.dims);
    vector<float> outData(m1.data);
    for(int i = 0; i < outData.size(); i++){
        outData[i] = m1.data[i]*m2.data[i];
    }
    return Matrix(outDims, outData);
}


Matrix Matrix::operator+ (Matrix const &obj){
    if(this->dims != obj.dims) throw "Matrices have different dimensions";

    vector<int> outDims(this->dims);
    vector<float> outData(this->data); 
    for(int i = 0; i < this->data.size(); i++){
        outData[i] += obj.data[i];
    }
    return Matrix(outDims, outData);
}

Matrix Matrix::operator* (float scalar){
    vector<int> outDims(this->dims);
    vector<float> outData(this->data);
    for(int i = 0; i < this->data.size(); i++){
        outData[i] *= scalar;
    }
    return Matrix(outDims, outData);
}