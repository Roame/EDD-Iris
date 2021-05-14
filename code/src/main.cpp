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
                    background = MatrixOps::edgeDetect(test);
                } else {
                    Matrix test = MatrixOps::downSample(pic);
                    Matrix edges = MatrixOps::edgeDetect(test);

                    std::vector<float> backData = background.getData();
                    std::vector<float> edgeData = edges.getData();
                    for(int i = 0; i < edgeData.size(); i++){
                        if(backData[i] == 1){
                            edgeData[i] = 0;
                        }
                    }
                    std::vector<int> edgeDims = edges.getDimensions();
                    edges = Matrix(edgeDims, edgeData);

                    // For displaying binary edge data
                    // std::vector<float> picData = pic.getData();
                    // std::vector<int> picDims = pic.getDimensions();
                    // edgeData = edges.getData();
                    // for(int i = 0; i < picDims[1]; i++){
                    //     for(int j = 0; j < picDims[0]; j++){
                    //         int index = 3*(i + j*picDims[1]);
                    //         int indexDown = (i/2+j/2*picDims[1]/2);
                    //         int val = edgeData[indexDown] == 1 ? 255 : 0;
                    //         picData[index] = val;
                    //         picData[index+1] = val;
                    //         picData[index+2] = val;
                    //     }
                    // }
                    // pic = Matrix(picDims, picData);

                    std::vector<int> xDists = ObjDetection::getXDistribution(edges);   
                    std::vector<int> yDists = ObjDetection::getYDistribution(edges);

                    std::vector<int> box = ObjDetection::getBox(xDists, yDists);
                    for(int i = 0; i < box.size(); i++){
                        box[i] *= 2;
                        std::cout << box[i] << " ";
                    }
                    std::cout << std::endl;
                    if(box.size() == 4){
                        pic = Draw::drawBox(pic, box[0], box[1], box[2], box[3]);
                        pic = Draw::drawBox(pic, box[0]-1, box[1]+1, box[2]-1, box[3]+1);
                    }

                }
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