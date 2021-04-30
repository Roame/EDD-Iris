#include <iostream>
#include <chrono>
#include <string.h>
#include <fstream>

#include "Capture.h"
#include "Matrix.h"
#include "toojpeg.h"
#include "ConfigManager.h"

using namespace std;

ofstream file;
int counter;
char jpegData[100000];

void myOutput(unsigned char oneByte){
    jpegData[counter] = oneByte;
    counter++;
}

void writeData(){
    file.open("Iris/web/images/feed.jpg", ios::binary);
    file.write(jpegData, sizeof(jpegData));
    file.close();
    counter = 0;
}

ConfigManager cfgManager;
struct config cConfig;

Matrix background, deltas, mask;
bool pFilterState = false;
int backCount = 0;
const int backMaxCount = 5;

const bool isRGB        = true;
const auto quality      = 90;
const bool downsample   = false;
const char* comment;

int main(){
    // vector<int> dims{2,2};
    // vector<float> d1{1,2,3.5,4}, d2{1,1,2,1};
    // Matrix m1(dims, d1), m2(dims, d2);
    // Matrix m3 = m2*0.5f + m1;
    // cout << m3.getData()[0] << endl;
    // cout << m3.getData()[1] << endl;
    // cout << m3.getData()[2] << endl;
    // cout << m3.getData()[3] << endl;

    Capture cap;
    cap.start();
    cap.takePicture();
    while(true){
        cConfig = cfgManager.getConfig("/home/pi/Iris/web/configs/cConfig.txt");

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        Matrix test = cap.takePicture();

        if(cConfig.configFound){
            if(cConfig.filterBack){
                if(cConfig.filterBack != pFilterState){
                    background = test*(1.0/backMaxCount);
                    backCount++;
                } else if(backCount < backMaxCount){
                    background = background + test * (1.0/backMaxCount); //Averaging
                    backCount++;
                } else if(backMaxCount >= backMaxCount){
                    deltas = test + (background * -1); //Subtraction
                    mask = Matrix::maskFromDeltas(deltas, 20);
                    test = Matrix::hadamardProduct(test, mask);
                }
            } else {
                if(cConfig.filterBack != pFilterState){
                    backCount = 0; // Resetting count
                }
            }
            pFilterState = cConfig.filterBack;
        }

        //Converting to char array for jpeg compression
        vector<float> data = test.getData();
        float* vData = data.data();
        char cData[320*240*3];
        for(int i = 0; i < sizeof(cData); i++){
            cData[i] = static_cast<int>(vData[i]);
        }

        TooJpeg::writeJpeg(myOutput, cData, 320, 240, isRGB, quality, downsample, comment);
        writeData();
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        cout    << "Time elapsed: " 
                << chrono::duration_cast<chrono::milliseconds>(end-begin).count()
                << " ms" << endl;
    }
    cap.stop();
}