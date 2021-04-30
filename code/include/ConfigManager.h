#pragma once

#include <fstream>
#include <string>
#include <iostream>

struct config {
    bool configFound = false;
    int hueMin = 0, 
        hueMax = 0,
        saturationMin = 0, 
        saturationMax = 0,
        valueMin = 0, 
        valueMax = 0;
    bool filterBack = false;

};

class ConfigManager
{
private:
    struct config cConfig;
public:
    ConfigManager();
    ~ConfigManager();
    config getConfig(const char* location);
};
