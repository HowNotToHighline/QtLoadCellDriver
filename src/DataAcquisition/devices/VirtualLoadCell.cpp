#include "VirtualLoadCell.h"

unsigned int VirtualLoadCell::UpdateData(float *buffer, unsigned int buffer_size) {
    int size_written = 0;

    while (size_written < buffer_size) {
        buffer[size_written++] =
                samples_generated > 100 && samples_generated < 600 ? static_cast<float>(samples_generated - 100) / 10.0f
                                                                   : 0;
        samples_generated++;
    }

    return size_written;
}
