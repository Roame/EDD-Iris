#pragma once

#include <fstream>
#include <string>

class FileReader
{
private:
public:
    FileReader();
    ~FileReader();
    static int getInt(const char * filename);
    static bool setInt(const char * filename, int val);
};
