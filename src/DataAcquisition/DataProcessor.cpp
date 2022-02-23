#include <stdexcept>
#include <filesystem>
#include <utility>
#include <iostream>
#include <src/DataAcquisition/devices/LabJackDriver.h>
#include <src/DataAcquisition/devices/VirtualLoadCell.h>
#include "DataProcessor.h"

void DataProcessor::Start(StartConfig startConfig, CallbackInterface *pWindow) {
    if(active_thread != nullptr) return;
    this->config = std::move(startConfig);
    ring_buffer_size = static_cast<int>(static_cast<float>(config.samplerate) * config.preRecordingTime);
    ring_buffer_index = 0;
    ring_buffer = new(std::nothrow) float[ring_buffer_size];
    shouldStop = false;
    active_thread = new std::thread(&DataProcessor::Run, this, pWindow);
}

void DataProcessor::Run(CallbackInterface *pWindow) {
    // Size the buffer so that there are 10 updates per second
    // If the samplerate is less than 10, let the buffer size be 1
    int buffer_size = std::max(config.samplerate / 10, 1);
    auto buffer = new(std::nothrow) float[buffer_size];

    bool triggered = false;
    int samplesBeforeStop = -1;
//    int samplesBeforeStop = 50;

    driver->StartStream(config.samplerate);

    while (samplesBeforeStop != 0 && !shouldStop) {
        unsigned int samples_available = driver->UpdateData(buffer, buffer_size);

        for (int i = 0; i < samples_available; ++i) {
            // If not triggered, check if this sample triggers
            // If it does, start the recording
            // Ff not, continue to the next sample
            if (!triggered) {
                if (buffer[i] >= config.triggerForce) {
                    triggered = true;
                    // TODO: Don't add non initialised data
                    DataBlockAdd(&ring_buffer[ring_buffer_index], ring_buffer_size - ring_buffer_index);
                    DataBlockAdd(&ring_buffer[0], ring_buffer_index);
                } else {
                    RingBufferPush(buffer[i]);
                    continue;
                }
            }
            // Triggered is guaranteed to be true after this

            if(samplesBeforeStop > 0) samplesBeforeStop--;
            else if (buffer[i] <= config.stopForce) samplesBeforeStop = config.samplerate * config.postRecordingTime;
            if(samplesBeforeStop == 0) break;

            // TODO: Make more efficient by adding the buffer in bulk
            DataBlockAdd(&buffer[i], 1);
        }
    }

    driver->StopStream();

    StoreToDisk();

    active_thread = nullptr;
    pWindow->stopped();
}

void DataProcessor::Stop() {
    if(active_thread == nullptr) return;
    shouldStop = true;
}

void DataProcessor::StoreToDisk() {
    printf("StoreToDisk\n");

//    printf("Storing to disk\n");
//    BufferBlock *tmp = first_buffer_block;
//    int blockNr = 0;
//    while (tmp != nullptr) {
//        printf("\nblockNr: %d\tcapacity: %u\tindex: %u\tdata:", blockNr, tmp->capacity, tmp->index);
//        for (int i = 0; i < tmp->index; ++i) {
//            printf(" %.2f", tmp->buffer[i]);
//        }
//        printf("\n");
//
//        blockNr++;
//        tmp = tmp->next;
//    }
//    printf("\nDone writing\n");

//    auto tmp = first_buffer_block;
//
//    time_t t = time(nullptr);
//    char timestamp_prefix[80];
//    strftime(timestamp_prefix, 80, "%d-%m-%Y_%H:%M:%S_", localtime(&t));
//    std::string directory = "./";
//    std::string file_name = directory + std::string(timestamp_prefix) + config.measurementName + ".csv";
//
//    // TODO: Deal with this better
//    if (std::filesystem::exists(file_name)) throw std::runtime_error("Log file already exists!");
//    std::ofstream new_file;
//    new_file.open(file_name);
//    if (!new_file) throw std::runtime_error("Log file creation failed");
}

void DataProcessor::RingBufferPush(float sample) {
    if (ring_buffer_size == 0) return;
    ring_buffer[ring_buffer_index++] = sample;
    ring_buffer_index %= ring_buffer_size;
}

void DataProcessor::DataBlockAdd(const float *buffer, unsigned int size) {
    if (first_buffer_block == nullptr || last_buffer_block == nullptr) {
        DataBlockCreate();
    }

    int written = 0;
    while (written < size) {
        unsigned int available = std::min(size, last_buffer_block->capacity - last_buffer_block->index);

        for (int i = 0; i < available; i++) {
            last_buffer_block->buffer[last_buffer_block->index++] = buffer[written++];
        }

        // If no more capacity in current data block, create a new one
        if (last_buffer_block->capacity == last_buffer_block->index) {
            DataBlockCreate();
        }
    }

}

void DataProcessor::DataBlockCreate() {
    auto prev_last_buffer_block = last_buffer_block;
    last_buffer_block = new BufferBlock;
    unsigned int size = 1000;
    last_buffer_block->buffer = new float[size];
    last_buffer_block->capacity = size;
    if (prev_last_buffer_block != nullptr) prev_last_buffer_block->next = last_buffer_block;
    if (first_buffer_block == nullptr) first_buffer_block = last_buffer_block;
}

void DataProcessor::Connect() {
    if(driver != nullptr) return;
//    driver = new VirtualLoadCell();
    driver = new LabJackDriver();
}

void DataProcessor::DisConnect() {
    if(driver == nullptr) return;
    delete driver;
    driver = nullptr;
}

void DataProcessor::Tare() {
    if(driver == nullptr) return;
    driver->Tare();
}

void DataProcessor::Calibrate(float force) {
    if(driver == nullptr) return;
    driver->Calibrate(force);
}