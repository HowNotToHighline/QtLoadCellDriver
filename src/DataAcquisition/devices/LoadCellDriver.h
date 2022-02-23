#ifndef DYNOSTREAM_LOADCELLDRIVER_H

class LoadCellDriver {
public:
    virtual void StartStream(unsigned int samplerate) = 0;

    virtual void StopStream() = 0;

    virtual unsigned int UpdateData(float *buffer, unsigned int buffer_size) = 0;

    virtual void Tare() = 0;

    virtual void Calibrate(float force) = 0;

    virtual ~LoadCellDriver() = default;
};

#define DYNOSTREAM_LOADCELLDRIVER_H

#endif //DYNOSTREAM_LOADCELLDRIVER_H
