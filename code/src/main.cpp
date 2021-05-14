#include <iostream>
#include <chrono>
#include <string.h>
#include <fstream>
#include <signal.h>

#include "Capture.h"
#include "Matrix.h"
#include "toojpeg.h"
#include "ConfigManager.h"
#include "PinControl.h"
#include "MatrixOps.h"
#include "ObjDetection.h"
#include "Draw.h"

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

Capture cap;

PinControl pctrl;

ConfigManager cfgManager;
struct config cConfig;

Matrix background, deltas, mask;
bool pFilterState = false;
int backCount = 0;
const int backMaxCount = 5;
vector<float> knownLines;



const bool isRGB        = true;
const auto quality      = 90;
const bool downsample   = false;
const char* comment;

void interruptCB(int signum){
    pctrl.stop();
    cap.stop();
    std::cout << endl << "All processes stopped" << endl;
    exit(signum);
}

int main(){
    // vector<int> dims{2,2};
    // vector<float> d1{1,2,3.5,4}, d2{1,1,2,1};
    // Matrix m1(dims, d1), m2(dims, d2);
    // Matrix m3 = m2*0.5f + m1;
    // cout << m3.getData()[0] << endl;
    // cout << m3.getData()[1] << endl;
    // cout << m3.getData()[2] << endl;
    // cout << m3.getData()[3] << endl;

    // Line filtering test
    // std::vector<float> test{    1,  1,
    //                             1,  1.01,
    //                             2,  1,
    //                             2.15, 1,
    //                             1.19, 4.5,
    //                             -1, 3,
    //                             -1, 4};
    // test = ObjDetection::getRectLines(4, test);
    // for(int i = 0; i < test.size()/2; i++){
    //     std::cout << test[2*i] << " " << test[2*i+1] << std::endl;
    // }

    signal(SIGINT, interruptCB);

    cap.start();
    cap.takePicture();
    while(true){
        cConfig = cfgManager.getConfig("/home/pi/Iris/web/configs/cConfig.txt");
        pctrl.setCycle((float) cConfig.LEDBrightness/100.0);

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        Matrix pic = cap.takePicture();

        if(cConfig.configFound){
            if(cConfig.filterBack){
                if(cConfig.filterBack != pFilterState){
                    Matrix test = MatrixOps::downSample(pic);
                    Matrix edges = MatrixOps::edgeDetect(test);
                    knownLines = ObjDetection::getLines(edges, std::vector<float>());
                } else {
                    Matrix test = MatrixOps::downSample(pic);
                    Matrix edges = MatrixOps::edgeDetect(test);

                    std::vector<float> data = pic.getData();
                    std::vector<int> dims = pic.getDimensions();
                    std::vector<float> edgeData = edges.getData();
                    for(int i = 0; i < dims[1]; i++){
                        for(int j = 0; j < dims[0]; j++){
                            int index = 3*(i + j*dims[1]);
                            int indexDown = (i/2+j/2*dims[1]/2);
                            int val = edgeData[indexDown] == 1 ? 255 : 0;
                            data[index] = val;
                            data[index+1] = val;
                            data[index+2] = val;
                        }
                    }
                    pic = Matrix(dims, data);

                    vector<float> lines = ObjDetection::getLines(edges, knownLines);
                    // lines = ObjDetection::getRectLines(knownLines.size(), lines);
                    for(int i = 0; i < lines.size()/2; i++){
                        std::vector<float> line{lines[2*i], lines[2*i+1]*2};
                        pic = Draw::drawLine(pic, line);
                        // std::cout << line[0] << " " << line[1] << std::endl;
                    }
                }
                // pic = MatrixOps::applyMask(test, edges);
            } else {
                if(cConfig.filterBack != pFilterState){
                    backCount = 0; // Resetting count
                }
            }
            pFilterState = cConfig.filterBack;
        }

        //Converting to char array for jpeg compression
        vector<float> data = pic.getData();
        float* vData = data.data();
        char cData[320*240*3];
        for(int i = 0; i < sizeof(cData); i++){
            cData[i] = static_cast<int>(vData[i]);
        }

        TooJpeg::writeJpeg(myOutput, cData, 320, 240, isRGB, quality, downsample, comment);
        writeData();
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        std::cout    << "Time elapsed: " 
                << chrono::duration_cast<chrono::milliseconds>(end-begin).count()
                << " ms" << endl;
    }
    cap.stop();
}