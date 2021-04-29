#pragma once

#include <fstream>
#include <string>
#include <iostream>

struct config {
    int hueMin, hueMax,
        saturationMin, saturationMax,
        valueMin, valueMax;
    bool filterBack;
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
