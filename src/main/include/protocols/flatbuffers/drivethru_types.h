#ifndef PROTOCOLS_FLATBUFFERS_TYPES_H
#define PROTOCOLS_FLATBUFFERS_TYPES_H

#include <cstdint>
#include "protocols/flatbuffers/drivethru_generated.h"

typedef struct AnalogReadResponse {
    uint8_t port;
    int16_t value;
} AnalogReadResponse;

typedef struct DigitalReadResponse {
    uint8_t port;
    bool value;
} DigitalReadResponse;

typedef struct ReadSubscribeRequest {
    uint8_t port;
    bool subscribe;
} ReadSubscribeRequest;

typedef struct ReadSubscribeResponse {
    ReadSubscribeRequest originalRequest;
    bool success;
} ReadSubscribeResponse;

typedef struct PinInformation {
    uint8_t physicalPin;
    bbfrc::msgs::PinMode mode;
    std::vector<bbfrc::msgs::PinMode> supportedModes;
    uint8_t analogChannel;
} PinInformation;

#endif
