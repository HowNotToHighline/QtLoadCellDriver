#include <cstdio>
#include <src/DataAcquisition/devices/labjack/LJM_Utilities.h>
#include <algorithm>
#include "LabJackDriver.h"

LabJackDriver::LabJackDriver() {
    printf("Connecting to labjack\n");
    handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");

    PrintDeviceInfoFromHandle(handle);
    printf("\n");
}

unsigned int LabJackDriver::UpdateData(float *buffer, unsigned int buffer_size) {
    double aData[buffer_size];
    int deviceScanBacklog = 0, LJMScanBacklog = 0;

    int err = LJM_eStreamRead(handle, aData, &deviceScanBacklog, &LJMScanBacklog);
    ErrorCheck(err, "LJM_eStreamRead");

    for (int i = 0; i < buffer_size; ++i) buffer[i] = rawToForce(aData[i]);

    return buffer_size;
}

LabJackDriver::~LabJackDriver() {
    printf("Closing labjack\n");
    if (handle >= 0) LJM_Close(handle);
}

void LabJackDriver::StartStream(unsigned int samplerate) {
    // TODO: Read excitation voltage
    // TODO: Set DAC voltage

    const int numChannels = 1;
    const char *CHANNEL_NAMES[numChannels] = {"AIN0"};
    int aScanList[numChannels];
    int err = LJM_NamesToAddresses(numChannels, CHANNEL_NAMES, aScanList, nullptr);
    ErrorCheck(err, "Getting positive channel addresses");

    // These are the defaults, but another program might've changed them already
    WriteNameOrDie(handle, "STREAM_TRIGGER_INDEX", 0);
    WriteNameOrDie(handle, "STREAM_CLOCK_SOURCE", 0);
    WriteNameOrDie(handle, "STREAM_SETTLING_US", 0);
    // These are definitely not defaults
    WriteNameOrDie(handle, "STREAM_RESOLUTION_INDEX", optimalResolutionIndex(samplerate));
    WriteNameOrDie(handle, "AIN0_NEGATIVE_CH", 1);
    WriteNameOrDie(handle, "AIN0_RANGE", 0.1);

    double scanRate = samplerate;
    int scansPerRead = static_cast<int>(std::max(samplerate / 10, 1u));
    err = LJM_eStreamStart(handle, scansPerRead, numChannels, aScanList, &scanRate);
    ErrorCheck(err, "LJM_eStreamStart");
    printf("Stream started. Actual scan rate: %.02f Hz (%.02f sample rate)\n", scanRate, scanRate * numChannels);
}

void LabJackDriver::StopStream() {
    int err = LJM_eStreamStop(handle);
    ErrorCheck(err, "Stopping stream");
}

void LabJackDriver::Tare() {
    double raw;
    WriteNameOrDie(handle, "AIN0_NEGATIVE_CH", 1);
    // TODO: 0.1 has lower resolution, but can use the full range
//    WriteNameOrDie(handle, "AIN0_RANGE", 0.1);
    WriteNameOrDie(handle, "AIN0_RANGE", 0.01);
    WriteNameOrDie(handle, "AIN0_RESOLUTION_INDEX", 8);
    int err = LJM_eReadName(handle, "AIN0", &raw);
    ErrorCheck(err, "LJM_eReadName");
    offset = raw;
    printf("offset: %f\n", offset);
}

void LabJackDriver::Calibrate(float force) {
    double raw;
    int err = LJM_eReadName(handle, "AIN0", &raw);
    ErrorCheck(err, "LJM_eReadName");
    scalar = (raw - offset) / force;
    printf("scalar: %f\n", scalar);
    // TODO: Store calibration in QSettings
}

float LabJackDriver::rawToForce(double raw) const {
//    return static_cast<float>((raw - offset) / scalar);
    return raw;
}

double LabJackDriver::optimalResolutionIndex(unsigned int samplerate) {
    // Why are stream acquisition times lower than single measurements?
    // https://labjack.com/support/datasheets/t-series/appendix-a-1#t7-stream-rates
    if(samplerate <= 600) {
        return 8;
    } else if(samplerate <= 1200) {
        return 7;
    } else if(samplerate <= 2500) {
        return 6;
    } else if(samplerate <= 5500) {
        return 5;
    } else if(samplerate <= 11000) {
        return 4;
    } else if(samplerate <= 22000) {
        return 3;
    } else if(samplerate <= 48000) {
        return 2;
    } else {
        return 1;
    }
}
