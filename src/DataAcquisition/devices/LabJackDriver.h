#ifndef LOADCELLDRIVER_LABJACKDRIVER_H
#define LOADCELLDRIVER_LABJACKDRIVER_H

#include <LabJackM.h>
#include "LoadCellDriver.h"

class LabJackDriver : public LoadCellDriver {
public:
    LabJackDriver();

    void StartStream(unsigned int samplerate) override;

    void StopStream() override;

    unsigned int UpdateData(float *buffer, unsigned int buffer_size) override;

    void Tare() override;

    void Calibrate(float force) override;

    ~LabJackDriver() override;

private:
    [[nodiscard]] float rawToForce(double raw) const;

    static double optimalResolutionIndex(unsigned int samplerate);

    int handle = -1;
    double offset = 0.000036;
    double scalar = 0.000325;
};


#endif //LOADCELLDRIVER_LABJACKDRIVER_H
