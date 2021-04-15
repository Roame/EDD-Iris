#include <iostream>
#include <sstream>

#include "Capture.h"

using namespace std;

int main(){
    Capture cap;
    cap.start();

    for(int i = 0; i < 30; i++){
        stringstream sstm;
        sstm << "/home/pi/Iris/web/images/image" << i << ".jpg";
        cout << "Starting picture " << i << endl;
        cap.takePicture(sstm.str().c_str());
        cout << "Done" << endl;
        usleep(1*1000000);
    }
    
    cap.stop();
}