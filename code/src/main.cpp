#include <iostream>

#include "Capture.h"

using namespace std;

int main(){
    Capture cap;
    cap.start();

    cout << "Starting picture 1" << endl;
    cap.takePicture("test0.jpg");
    cout << "Finished" << endl;

    usleep(4*1000000);
    
    cout << "Starting picture 2" << endl;
    cap.takePicture("test1.jpg");
    cout << "Finished" << endl;
    
    cap.stop();
}