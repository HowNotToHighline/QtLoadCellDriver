#ifndef LOADCELLDRIVER_VIRTUALLOADCELL_H
#define LOADCELLDRIVER_VIRTUALLOADCELL_H


#include "LoadCellDriver.h"

class VirtualLoadCell : public LoadCellDriver {
public:
    void StartStream(unsigned int samplerate) override;

    unsigned int UpdateData(float *buffer, unsigned int buffer_size) override;

private:
    int samples_generated = 0;
    unsigned int _samplerate = 0;
};


#endif //LOADCELLDRIVER_VIRTUALLOADCELL_H
