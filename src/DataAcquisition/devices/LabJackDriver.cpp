#include <cstdio>
#include <src/DataAcquisition/devices/labjack/LJM_Utilities.h>
#include <algorithm>
#include "LabJackDriver.h"

LabJackDriver::LabJackDriver(unsigned int samplerate) {
    printf("Connecting to labjack\n");
    handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");

    PrintDeviceInfoFromHandle(handle);
    printf("\n");

// TODO: Configure differential mode
// TODO: Read excitation voltage

    const int numChannels = 1;
    const char *CHANNEL_NAMES[numChannels] = {"AIN0"};
    int aScanList[numChannels];
    int err = LJM_NamesToAddresses(numChannels, CHANNEL_NAMES, aScanList, nullptr);
    ErrorCheck(err, "Getting positive channel addresses");

//    HardcodedConfigureStream(handle);

    double scanRate = samplerate;
    int scansPerRead = static_cast<int>(std::max(samplerate / 10, 1u));
    err = LJM_eStreamStart(handle, scansPerRead, numChannels, aScanList, &scanRate);
    ErrorCheck(err, "LJM_eStreamStart");
    printf("Stream started. Actual scan rate: %.02f Hz (%.02f sample rate)\n", scanRate, scanRate * numChannels);
}

unsigned int LabJackDriver::UpdateData(float *buffer, unsigned int buffer_size) {
    double aData[buffer_size];
    int deviceScanBacklog = 0, LJMScanBacklog = 0;

    int err = LJM_eStreamRead(handle, aData, &deviceScanBacklog, &LJMScanBacklog);
    ErrorCheck(err, "LJM_eStreamRead");

    // TODO: Scale and offset
    for (int i = 0; i < buffer_size; ++i) buffer[i] = static_cast<float>(aData[i]);

    return buffer_size;
}

LabJackDriver::~LabJackDriver() {
    printf("Closing labjack\n");
    if (handle >= 0) LJM_Close(handle);
}
