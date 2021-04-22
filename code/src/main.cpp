#include <iostream>
#include <chrono>

#include "Capture.h"
#include "Matrix.h"

using namespace std;

int main(){
    // vector<int> dims{2,2,2};
    // vector<float> data{1.00001123,2,3,4,5,6,7,8};

    // Matrix mat(dims, data);
    // cout << mat.toString() << endl;

    // std::vector<int> test = {1,2,3,4,5,6,7,8};
    // cout << test[0] << endl;

    Capture cap;
    cap.start();
    cout << "Taking picture" << endl;
    Matrix test = cap.takePicture();
    cap.stop();
    cout << "Printing string rep:" << endl;
    cout << test.toString() << endl;

    // Capture cap;
    // cap.start();
    // while(true){
    //     chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    //     cap.takePicture("/home/pi/Iris/web/images/feed.jpg");
    //     chrono::steady_clock::time_point end = chrono::steady_clock::now();

    //     cout    << "Time elapsed: " 
    //             << chrono::duration_cast<chrono::milliseconds>(end-begin).count()
    //             << " ms" << endl;
    // }
    // cap.stop();
    return 0;
}