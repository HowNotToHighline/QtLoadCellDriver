#ifndef LOADCELLDRIVER_DATAPROCESSOR_H
#define LOADCELLDRIVER_DATAPROCESSOR_H


#include <string>
#include <fstream>
#include <thread>
#include <src/CallbackInterface.h>

struct StartConfig {
    void *loadCellConfig;
    int samplerate;
    float triggerForce;
    float stopForce;
    float preRecordingTime;
    float postRecordingTime;
    std::string measurementName;
};

class DataProcessor {
public:
    DataProcessor() = default;

    void Start(StartConfig startConfig, CallbackInterface *pWindow);

    void Stop();

    struct BufferBlock {
        unsigned int capacity = 0;
        unsigned int index = 0;
        float *buffer = nullptr;
        BufferBlock *next = nullptr;
    };

private:
    void Run(CallbackInterface *pWindow);

    void StoreToDisk();

    void RingBufferPush(float sample);

    void DataBlockAdd(const float *buffer, unsigned int size);

    void DataBlockCreate();

    StartConfig config;

    unsigned int ring_buffer_size = 0;
    unsigned int ring_buffer_index = 0;
    float *ring_buffer = nullptr;

    BufferBlock *first_buffer_block = nullptr;
    BufferBlock *last_buffer_block = nullptr;

    std::thread *active_thread = nullptr;
    bool shouldStop = false;
};


#endif //LOADCELLDRIVER_DATAPROCESSOR_H
