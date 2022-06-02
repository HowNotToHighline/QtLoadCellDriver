#ifndef LOADCELLDRIVER_LOADCELLDRIVERFACTORY_H
#define LOADCELLDRIVER_LOADCELLDRIVERFACTORY_H


#include <string>
#include <map>
#include "LoadCellDriver.h"

class LoadCellDriverFactory {
public:
    static bool isValidConfig(std::string driver, const std::map<std::string, std::string> &options);

    static LoadCellDriver *createDriver(std::string driver, const std::map<std::string, std::string> &options);
};


#endif //LOADCELLDRIVER_LOADCELLDRIVERFACTORY_H
