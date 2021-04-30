#include "ConfigManager.h"

int getInt(std::string &inString, int startPos){
    return std::stoi(inString.substr(startPos, inString.length()-startPos));
}

bool getBool(std::string &inString, int startPos){
    std::string text = inString.substr(startPos, inString.length()-startPos);
    if(text == "on") return true;
    return false;
}

ConfigManager::ConfigManager(){}

ConfigManager::~ConfigManager(){}

config ConfigManager::getConfig(const char* location){
    std::ifstream cfgFile(location);
    std::string line;
    if(cfgFile.is_open()){
        cConfig.configFound = true;
        while(getline(cfgFile, line)){
            for(int i = 0; i < line.length(); i++){
                if(line[i] == (char) '#') break;
                if(line[i] == (char) ':'){
                    std::string field = line.substr(0, i);
                    if(field == "Hue Min") { cConfig.hueMin = getInt(line, i+2); break; }
                    if(field == "Hue Max") { cConfig.hueMax = getInt(line, i+2); break; }
                    if(field == "Saturation Min") { cConfig.saturationMin = getInt(line, i+2); break; }
                    if(field == "Saturation Max") { cConfig.saturationMax = getInt(line, i+2); break; }
                    if(field == "Value Min") { cConfig.valueMin = getInt(line, i+2); break; }
                    if(field == "Value Max") { cConfig.valueMax = getInt(line, i+2); break; }
                    if(field == "Filter Background") { cConfig.filterBack = getBool(line, i + 2); break; }
                    break;
                }
            }
        }
    } else {
        cConfig.configFound = false;
    }
    cfgFile.close();
    return cConfig;
}
