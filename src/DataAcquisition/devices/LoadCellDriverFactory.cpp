#include "LoadCellDriverFactory.h"

bool LoadCellDriverFactory::isValidConfig(std::string driver, const std::map<std::string, std::string> &options) {
    if("labjack" == driver) {
        if(!options.contains("deviceType")) return false;
        if(!options.contains("connectionType")) return false;
        if(!options.contains("identifier")) return false;
        if(!options.contains("offset")) return false;
        if(!options.contains("scale")) return false;
        return true;
    } else {
        return false;
    }
}

LoadCellDriver *
LoadCellDriverFactory::createDriver(std::string driver, const std::map<std::string, std::string> &options) {
    return nullptr;
}
