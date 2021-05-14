#include "FileReader.h"


int FileReader::getInt(const char * filename){
    std::ifstream file(filename);
    int output = 0;
    std::string line;
    if(file.is_open()){
        while(getline(file, line)){
            output = std::stoi(line.substr(0, 1));
        }
    }
    file.close();
    return output;
}

bool FileReader::setInt(const char * filename, int val){
    std::ofstream file(filename);
    if(file.is_open()){
        file << val;
        file.close();
        return true;
    } else {
        return false;
    }
}