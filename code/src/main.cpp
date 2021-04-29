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

int main(){
    ConfigManager cfgManager;
    struct config cConfig;
    cConfig = cfgManager.getConfig("/home/pi/Iris/web/configs/sample.txt");
    cout << cConfig.hueMax << endl;
    cout << cConfig.hueMin << endl;
    cout << cConfig.saturationMin << endl;
    cout << cConfig.saturationMax << endl;
    cout << cConfig.valueMin << endl;
    cout << cConfig.valueMax << endl;
    cout << cConfig.filterBack << endl;



    // const bool isRGB        = true;
    // const auto quality      = 90;
    // const bool downsample   = false;
    // const char* comment;


    // Capture cap;
    // cap.start();
    // cap.takePicture();
    // while(true){

    //     chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    //     Matrix test = cap.takePicture();
    //     vector<float> data = test.getData();
    //     float* vData = data.data();
    //     char cData[320*240*3];
    //     for(int i = 0; i < sizeof(cData); i++){
    //         cData[i] = static_cast<int>(vData[i]);
    //     }

    //     TooJpeg::writeJpeg(myOutput, cData, 320, 240, isRGB, quality, downsample, comment);
    //     writeData();
    //     chrono::steady_clock::time_point end = chrono::steady_clock::now();

    //     cout    << "Time elapsed: " 
    //             << chrono::duration_cast<chrono::milliseconds>(end-begin).count()
    //             << " ms" << endl;
    // }
    // cap.stop();
}