#ifndef PROTOCOLS_FLATBUFFERS_UTIL_H
#define PROTOCOLS_FLATBUFFERS_UTIL_H

#include <cstdint>
#include <flatbuffers/flatbuffers.h>
#include "drivethru_generated.h"
#include "drivethru_types.h"

class FlatBuffersUtil {
    public:
        // Analog
        static uint8_t* makeAnalogReadRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int* bufferSize);
        static uint8_t* makeAnalogReadResponse(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int16_t value, int* bufferSize);
        static uint8_t* makeAnalogReadMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<uint8_t> ports, int* bufferSize);
        static uint8_t* makeAnalogReadMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<AnalogReadResponse> responses, int* bufferSize);
        static uint8_t* makeAnalogReadSubscribeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool subscribe, int* bufferSize);
        static uint8_t* makeAnalogReadSubscribeResponse(flatbuffers::FlatBufferBuilder& builder, ReadSubscribeRequest originalRequest, bool success, int* bufferSize);
        static uint8_t* makeAnalogReadSubscribeMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeRequest> requests, int* bufferSize);
        static uint8_t* makeAnalogReadSubscribeMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeResponse> responses, int* bufferSize);

        // Digital
        static uint8_t* makeDigitalWriteRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool value, int* bufferSize);
        static uint8_t* makeDigitalReadRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int* bufferSize);
        static uint8_t* makeDigitalReadResponse(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool value, int* bufferSize);
        static uint8_t* makeDigitalReadMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<uint8_t> ports, int* bufferSize);
        static uint8_t* makeDigitalReadMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<DigitalReadResponse> responses, int* bufferSize);
        static uint8_t* makeDigitalReadSubscribeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool subscribe, int* bufferSize);
        static uint8_t* makeDigitalReadSubscribeResponse(flatbuffers::FlatBufferBuilder& builder, ReadSubscribeRequest originalRequest, bool success, int* bufferSize);
        static uint8_t* makeDigitalReadSubscribeMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeRequest> requests, int* bufferSize);
        static uint8_t* makeDigitalReadSubscribeMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeResponse> responses, int* bufferSize);

        // Servo
        static uint8_t* makeSetServoAngleRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t servoPort, uint16_t angle, int* bufferSize);

        // System
        static uint8_t* makeGetPinInformationRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize);
        static uint8_t* makeGetPinInformationResponse(flatbuffers::FlatBufferBuilder& builder, uint16_t totalPins, std::vector<PinInformation> digitalPins, std::vector<PinInformation> analogPins, std::vector<PinInformation> servoPins, int* bufferSize);
        static uint8_t* makeGetFirmwareNameAndVersionRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize);
        static uint8_t* makeGetFirmwareNameAndVersionResponse(flatbuffers::FlatBufferBuilder& builder, std::string name, uint8_t versionMajor, uint8_t versionMinor, int* bufferSize);
        static uint8_t* makeSetDigitalPinModeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bbfrc::msgs::PinMode pinMode, int* bufferSize);
        static uint8_t* makeSystemResetRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize);
    private:
        static uint8_t* wrapMessage(flatbuffers::FlatBufferBuilder& builder, flatbuffers::Offset<void> payloadOffset, bbfrc::msgs::Payload type, int* bufferSize);
};

void makePinInfo(flatbuffers::FlatBufferBuilder& builder, std::vector<flatbuffers::Offset<bbfrc::msgs::PinInformation>>& pinInfoVector, std::vector<PinInformation> pinInfos);

#endif
