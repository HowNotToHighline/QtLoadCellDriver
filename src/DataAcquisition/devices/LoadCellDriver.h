#ifndef DYNOSTREAM_LOADCELLDRIVER_H

class LoadCellDriver {
public:
    virtual unsigned int UpdateData(float *buffer, unsigned int buffer_size) = 0;

    virtual ~LoadCellDriver() = default;
};

#define DYNOSTREAM_LOADCELLDRIVER_H

#endif //DYNOSTREAM_LOADCELLDRIVER_H
