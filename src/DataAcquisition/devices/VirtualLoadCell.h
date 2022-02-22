#ifndef LOADCELLDRIVER_VIRTUALLOADCELL_H
#define LOADCELLDRIVER_VIRTUALLOADCELL_H


#include "LoadCellDriver.h"

class VirtualLoadCell : public LoadCellDriver {
public:
    unsigned int UpdateData(float *buffer, unsigned int buffer_size) override;

private:
    int samples_generated = 0;
};


#endif //LOADCELLDRIVER_VIRTUALLOADCELL_H
