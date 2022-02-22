#ifndef LOADCELLDRIVER_LABJACKDRIVER_H
#define LOADCELLDRIVER_LABJACKDRIVER_H

#include <LabJackM.h>
#include "LoadCellDriver.h"

class LabJackDriver : public LoadCellDriver {
public:
    explicit LabJackDriver(unsigned int samplerate);

    unsigned int UpdateData(float *buffer, unsigned int buffer_size) override;

    ~LabJackDriver() override;

private:
    int handle = -1;
};


#endif //LOADCELLDRIVER_LABJACKDRIVER_H
