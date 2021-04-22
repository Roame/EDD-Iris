#include "Matrix.h"

using namespace std;

Matrix::Matrix(){}

Matrix::Matrix(vector<int> &dimensions, vector<float> &data){
    this->data = data;
    this->dims = dimensions;

}

Matrix::~Matrix(){
    
}


//Currently experiencing memory issues when printing large arrays
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

vector<float> Matrix::getData(){
    return data;
}