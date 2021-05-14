#include "ObjDetection.h"

bool checkProximity(float x, float y, std::vector<float> &lines){
    // Returns true if the point is close to line
    float threshold = 5; // How many pixels away is considered close enough to be a part of the line
    for(int k = 0; k < lines.size()/2; k++){
        float a = lines[k], b = lines[k+1];
        float intersectX = (a/(a*a + 1)) * (y + (1.0/a)*x - b);
        float intersectY = a*intersectX + b;
        float dist = sqrt(pow(intersectX-x, 2)+ pow(intersectY-y, 2));
        if(dist <= threshold){
            return true;
        }
    }
    return false;
}

std::vector<float> constructLine(Matrix &binImg, int x, int y){
    std::vector<int> dims = binImg.getDimensions();
    std::vector<float> data = binImg.getData();

    double radius = 5;
    int circleCount = 30;
    std::vector<int> circle;
    for(double t = 0; t < 2*M_PI; t+=(2*M_PI/circleCount)){
        circle.push_back((int)round(cos(t)*radius));
        circle.push_back((int)round(sin(t)*radius));
    }

    bool headingInitialized;
    double heading;
    int startX = x, startY = y, count = 0, minCount = 9;
    bool running = true;
    while(running){
        bool success = false;
        for(int i = 0; i < circleCount && !success; i++){
            int circleX = circle[2*i];
            int circleY = circle[2*i+1];

            if(headingInitialized){
                // std::cout << circleX << " " << circleY << " " << heading << std::endl;
                float diff = std::abs(atan2(circleY, circleX)-heading);
                diff = diff > M_PI ? 2*M_PI-diff : diff;
                
                if(diff > 60*M_PI/180){
                    // std::cout << "Too different" << std::endl;
                    continue;
                }
            }

            int testX = x + circleX;
            int testY = y + circleY;
            int index = testX + testY * dims[1];
            if(data[index] == 1){
                heading = atan2(testY-startY, testX-startX);
                headingInitialized = true;
                x = testX;
                y = testY;
                count++;
                success = true;
            }
        }
        if(!success) running = false;
    }

    if(count < minCount) return std::vector<float>();

    float slope = x-startX != 0 ? (y-startY)/(x-startX) : 1000;
    float inter = (startY-slope*startX);
    return std::vector<float>{slope, inter};

}

std::vector<float> ObjDetection::getLines(Matrix &binaryImg, std::vector<float> knownLines){
    std::vector<int> dims = binaryImg.getDimensions();
    std::vector<float> data = binaryImg.getData();

    for(int i = 0; i < dims[1]; i++){
        for(int j = 0; j < dims[0]; j++){
            int index = (j*dims[1]+i);
            if(data[index] == 1 && !checkProximity(i, j, knownLines)){
                std::vector<float> line = constructLine(binaryImg, i, j);
                knownLines.insert(knownLines.begin(), line.begin(), line.end());
            }
        }
    }
    return knownLines;
}

std::vector<float> ObjDetection::getRectLines(int filterIndex, std::vector<float> newLines){
    // Removing pre-existing lines:
    newLines.erase(newLines.begin(), newLines.begin()+filterIndex);

    // Searching for duplicates in remaining set
    std::vector<int> dupIndices;
    int numLines = newLines.size()/2;
    for(int i = 0; i < numLines; i++){
        if(std::find(dupIndices.begin(), dupIndices.end(), i*2) != dupIndices.end()) continue;
        
        for(int j = i+1; j < numLines; j++){
            float slopeDiff = std::abs(newLines[i*2]-newLines[j*2]);
            float interDiff = std::abs(newLines[i*2+1]-newLines[j*2+1]);
            if((slopeDiff < 1.0*newLines[i*2] || slopeDiff == 0) && (interDiff < 0.2*newLines[i*2+1] || interDiff == 0)){
                dupIndices.push_back(j*2);
            }
        }
    }

    // Erasing duplicate lines
    int totalI = dupIndices.size();
    for(int i = totalI-1; i >= 0; i--){
        newLines.erase(newLines.begin()+dupIndices[i], newLines.begin()+dupIndices[i]+2);
    }

    // Cross-referencing to find parallel lines
    std::vector<int> parallelIndices;

    numLines = newLines.size()/2;
    for(int i = 0; i < numLines; i++){
        for(int j = i+1; j < numLines; j++){
            float slopeDiff = std::abs(newLines[i*2]-newLines[j*2]);
            if(slopeDiff < 1.0*newLines[i*2] || slopeDiff == 0){
                parallelIndices.push_back(i*2);
                parallelIndices.push_back(j*2);
                i++; j = i+1;
                // Trying to avoid chains of pairs, might cause
                // issues if there are 3 or more parallel lines
            }
        }
    }

    // Extracting parallel lines and returning
    std::vector<float> output;
    for(int i = 0; i < parallelIndices.size(); i++){
        output.push_back(newLines[parallelIndices[i]]);
        output.push_back(newLines[parallelIndices[i]+1]);
    }
    return output;
}

std::vector<int> ObjDetection::getXDistribution(Matrix &edges){
    std::vector<int> edgeDims = edges.getDimensions();
    std::vector<float> edgeData = edges.getData();

    std::vector<int> distVals;

    for(int i = 0; i < edgeDims[1]; i++){
        int lowY, highY;
        bool success = false;
        for(int j = 0; j < edgeDims[0]; j++){
            int index = i+j*edgeDims[1];
            if(edgeData[index] == 1){
                lowY = j;
                success = true;
                break;
            }
        }
        if(!success){
            distVals.push_back(0);
            continue;
        }
        for(int j = edgeDims[0]-1; j >= 0; j--){
            int index = i+j*edgeDims[1];
            if(edgeData[index] == 1){
                highY = j;
                break;
            }
        }
        distVals.push_back(highY-lowY);
    }
    return distVals;
}

std::vector<int> ObjDetection::getYDistribution(Matrix &edges){
    std::vector<int> edgeDims = edges.getDimensions();
    std::vector<float> edgeData = edges.getData();

    std::vector<int> distVals;

    for(int j = 0; j < edgeDims[0]; j++){
        int lowX, highX;
        bool success = false;
        for(int i = 0; i < edgeDims[1]; i++){
            int index = i+j*edgeDims[1];
            if(edgeData[index] == 1){
                lowX = i;
                success = true;
                break;
            }
        }
        if(!success){
            distVals.push_back(0);
            continue;
        }
        for(int i = edgeDims[1]-1; i >= 0; i--){
            int index = i+j*edgeDims[1];
            if(edgeData[index] == 1){
                highX = i;
                break;
            }
        }
        distVals.push_back(highX-lowX);
    }
    return distVals;
}

std::vector<int> ObjDetection::getCorners(std::vector<int> &distX, std::vector<int> &distY, Matrix &edges){
    std::vector<int> output;
    std::vector<int> dims = edges.getDimensions();
    std::vector<float> data = edges.getData();
    int plus[] = {
        0, 0, 0, 1, 1, 0, 0, -1, -1, 0
    };

    for(int a = 1; a < distX.size(); a++){
        if(distX[a-1] > 0 && distX[a+1] > 0 && distX[a+1] > 0){
            output.push_back(a);
            for(int j = 0; j < dims[0]; j++){
                int count = 0;
                for(int k = 0; k < sizeof(plus)/sizeof(int)/2; k++){
                    int index = (a+plus[2*k])+(j+plus[2*k+1])*dims[1];
                    if(data[index] == 1){
                        count++;
                    }
                }
                if(count > 2) {
                    output.push_back(j);
                    break;
                }
            }
            break;
        }
    }

    for(int a = distX.size()-2; a >= 1; a--){
        if(distX[a-1] > 0 && distX[a] > 0 && distX[a+1] > 0){
            output.push_back(a);
            for(int j = 0; j < dims[0]; j++){
                int count = 0;
                for(int k = 0; k < sizeof(plus)/sizeof(int)/2; k++){
                    int index = (a+plus[2*k])+(j+plus[2*k+1])*dims[1];
                    if(data[index] == 1){
                        count++;
                    }
                }
                if(count > 2) {
                    output.push_back(j);
                    break;
                }
            }
            break;
        }
    }


    // For y:
    for(int a = 1; a < distY.size(); a++){
        if(distY[a-1] > 0 && distY[a] > 0 && distY[a+1] > 0){
            for(int i = 0; i < dims[0]; i++){
                int count = 0;
                for(int k = 0; k < sizeof(plus)/sizeof(int)/2; k++){
                    int index = (i+plus[2*k])+(a+plus[2*k+1])*dims[1];
                    if(data[index] == 1){
                        count++;
                    }
                }
                if(count > 2) {
                    output.push_back(i);
                    break;
                }
            }
            output.push_back(a);
            break;
        }
    }

    for(int a = distY.size()-2; a >= 1; a--){
        if(distY[a-1] > 0 && distY[a] > 0 && distY[a+1] > 0){
            for(int i = 0; i < dims[0]; i++){
                int count = 0;
                for(int k = 0; k < sizeof(plus)/sizeof(int)/2; k++){
                    int index = (i+plus[2*k])+(a+plus[2*k+1])*dims[1];
                    if(data[index] == 1){
                        count++;
                    }
                }
                if(count > 2) {
                    output.push_back(i);
                    break;
                }
            }
            output.push_back(a);
            break;
        }
    }
    return output;
}

std::vector<int> ObjDetection::getBox(std::vector<int> &distX, std::vector<int> &distY){
    std::vector<int> output;

    for(int a = 0; a < distX.size()-2; a++){
        if(distX[a] > 0 && distX[a+1] > 0 && distX[a+2] > 0){
            output.push_back(a);
            break;
        }
    }

    for(int a = distX.size()-1; a >= 2; a--){
        if(distX[a] > 0 && distX[a-1] > 0 && distX[a-2] > 0){
            output.push_back(a);
            break;
        }
    }

    for(int a = 0; a < distY.size()-2; a++){
        if(distY[a] > 0 && distY[a+1] > 0 && distY[a+2] > 0){
            output.push_back(a); 
            break;  
        }
    }

    for(int a = distY.size()-1; a >= 2; a--){
        if(distY[a] > 0 && distY[a-1] > 0 && distY[a-2] > 0){
            output.push_back(a);
            break;
        }
    }
    return output;
}
