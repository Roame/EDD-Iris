#include <iostream>
#include <chrono>

#include "Capture.h"

using namespace std;

int main(){
    Capture cap;
    cap.start();

    while(true){
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        cap.takePicture("/home/pi/Iris/web/images/feed.jpg");
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        cout    << "Time elapsed: " 
                << chrono::duration_cast<chrono::milliseconds>(end-begin).count()
                << " ms" << endl;
    }
    
    cap.stop();
}