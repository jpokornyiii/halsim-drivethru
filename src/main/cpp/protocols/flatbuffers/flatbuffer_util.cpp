#include "protocols/flatbuffers/flatbuffer_util.h"

#include <iostream>

// ANALOG MESSAGES
uint8_t* FlatBuffersUtil::makeAnalogReadRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int* bufferSize) {
    auto req = bbfrc::msgs::CreateAnalogReadRequest(builder, port);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_AnalogReadRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadResponse(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int16_t value, int* bufferSize) {
    auto resp = bbfrc::msgs::CreateAnalogReadResponse(builder, port, value);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_AnalogReadResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<uint8_t> ports, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::AnalogReadRequest>> requestOffsets;
    for (auto it = ports.begin(); it != ports.end(); it++) {
        auto req = bbfrc::msgs::CreateAnalogReadRequest(builder, *it);
        requestOffsets.push_back(req);
    }

    auto vec = builder.CreateVector(requestOffsets);
    auto req = bbfrc::msgs::CreateAnalogReadMultiRequest(builder, vec);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_AnalogReadMultiRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<AnalogReadResponse> responses, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::AnalogReadResponse>> responseOffsets;
    for (auto it = responses.begin(); it != responses.end(); it++) {
        auto resp = bbfrc::msgs::CreateAnalogReadResponse(builder, it->port, it->value);
        responseOffsets.push_back(resp);
    }

    auto vec = builder.CreateVector(responseOffsets);
    auto resp = bbfrc::msgs::CreateAnalogReadMultiResponse(builder, vec);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_AnalogReadMultiResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadSubscribeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool subscribe, int* bufferSize) {
    auto req = bbfrc::msgs::CreateAnalogReadSubscribeRequest(builder, port, subscribe);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_AnalogReadSubscribeRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadSubscribeResponse(flatbuffers::FlatBufferBuilder& builder, ReadSubscribeRequest originalRequest, bool success, int* bufferSize) {
    auto origReq = bbfrc::msgs::CreateAnalogReadSubscribeRequest(builder, originalRequest.port, originalRequest.subscribe);
    auto req = bbfrc::msgs::CreateAnalogReadSubscribeResponse(builder, origReq);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_AnalogReadSubscribeResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadSubscribeMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeRequest> requests, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::AnalogReadSubscribeRequest>> requestOffsets;
    for (auto it = requests.begin(); it != requests.end(); it++) {
        auto req = bbfrc::msgs::CreateAnalogReadSubscribeRequest(builder, it->port, it->subscribe);
        requestOffsets.push_back(req);
    }

    auto vec = builder.CreateVector(requestOffsets);
    auto req = bbfrc::msgs::CreateAnalogReadSubscribeMultiRequest(builder, vec);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_AnalogReadSubscribeMultiRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeAnalogReadSubscribeMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeResponse> responses, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::AnalogReadSubscribeResponse>> responseOffsets;
    for (auto it = responses.begin(); it != responses.end(); it++) {
        auto originalRequest = bbfrc::msgs::CreateAnalogReadSubscribeRequest(builder, it->originalRequest.port, it->originalRequest.subscribe);
        auto resp = bbfrc::msgs::CreateAnalogReadSubscribeResponse(builder, originalRequest, it->success);
        responseOffsets.push_back(resp);
    }

    auto vec = builder.CreateVector(responseOffsets);
    auto resp = bbfrc::msgs::CreateAnalogReadSubscribeMultiResponse(builder, vec);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_AnalogReadSubscribeMultiResponse, bufferSize);
}

// DIGITAL MESSAGES
uint8_t* FlatBuffersUtil::makeDigitalWriteRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool value, int* bufferSize) {
    auto req = bbfrc::msgs::CreateDigitalWriteRequest(builder, port, value);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalWriteRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, int* bufferSize) {
    auto req = bbfrc::msgs::CreateDigitalReadRequest(builder, port);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadResponse(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool value, int* bufferSize) {
    auto req = bbfrc::msgs::CreateDigitalReadResponse(builder, port, value);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<uint8_t> ports, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::DigitalReadRequest>> requestOffsets;
    for (auto it = ports.begin(); it != ports.end(); it++) {
        auto req = bbfrc::msgs::CreateDigitalReadRequest(builder, *it);
        requestOffsets.push_back(req);
    }

    auto vec = builder.CreateVector(requestOffsets);
    auto req = bbfrc::msgs::CreateDigitalReadMultiRequest(builder, vec);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadMultiRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<DigitalReadResponse> responses, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::DigitalReadResponse>> responseOffsets;
    for (auto it = responses.begin(); it != responses.end(); it++) {
        auto resp = bbfrc::msgs::CreateDigitalReadResponse(builder, it->port, it->value);
        responseOffsets.push_back(resp);
    }

    auto vec = builder.CreateVector(responseOffsets);
    auto resp = bbfrc::msgs::CreateDigitalReadMultiResponse(builder, vec);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_DigitalReadMultiResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadSubscribeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bool subscribe, int* bufferSize) {
    auto req = bbfrc::msgs::CreateDigitalReadSubscribeRequest(builder, port, subscribe);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadSubscribeRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadSubscribeResponse(flatbuffers::FlatBufferBuilder& builder, ReadSubscribeRequest originalRequest, bool success, int* bufferSize) {
    auto origReq = bbfrc::msgs::CreateDigitalReadSubscribeRequest(builder, originalRequest.port, originalRequest.subscribe);
    auto req = bbfrc::msgs::CreateDigitalReadSubscribeResponse(builder, origReq);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadSubscribeResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadSubscribeMultiRequest(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeRequest> requests, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::DigitalReadSubscribeRequest>> requestOffsets;
    for (auto it = requests.begin(); it != requests.end(); it++) {
        auto req = bbfrc::msgs::CreateDigitalReadSubscribeRequest(builder, it->port, it->subscribe);
        requestOffsets.push_back(req);
    }

    auto vec = builder.CreateVector(requestOffsets);
    auto req = bbfrc::msgs::CreateDigitalReadSubscribeMultiRequest(builder, vec);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_DigitalReadSubscribeMultiRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeDigitalReadSubscribeMultiResponse(flatbuffers::FlatBufferBuilder& builder, std::vector<ReadSubscribeResponse> responses, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::DigitalReadSubscribeResponse>> responseOffsets;
    for (auto it = responses.begin(); it != responses.end(); it++) {
        auto originalRequest = bbfrc::msgs::CreateDigitalReadSubscribeRequest(builder, it->originalRequest.port, it->originalRequest.subscribe);
        auto resp = bbfrc::msgs::CreateDigitalReadSubscribeResponse(builder, originalRequest, it->success);
        responseOffsets.push_back(resp);
    }

    auto vec = builder.CreateVector(responseOffsets);
    auto resp = bbfrc::msgs::CreateDigitalReadSubscribeMultiResponse(builder, vec);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_DigitalReadSubscribeMultiResponse, bufferSize);
}


// SERVO
uint8_t* FlatBuffersUtil::makeSetServoAngleRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t servoPort, uint16_t angle, int* bufferSize) {
    auto req = bbfrc::msgs::CreateSetServoAngleRequest(builder, servoPort, angle);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_SetServoAngleRequest, bufferSize);
}


// SYSTEM
uint8_t* FlatBuffersUtil::makeGetPinInformationRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize) {
    auto req = bbfrc::msgs::CreateGetPinInformationRequest(builder);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_GetPinInformationRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeGetPinInformationResponse(flatbuffers::FlatBufferBuilder& builder, uint16_t totalPins, std::vector<PinInformation> digitalPins, std::vector<PinInformation> analogPins, std::vector<PinInformation> servoPins, int* bufferSize) {
    std::vector<flatbuffers::Offset<bbfrc::msgs::PinInformation>> digitalPinOffsets;
    std::vector<flatbuffers::Offset<bbfrc::msgs::PinInformation>> analogPinOffsets;
    std::vector<flatbuffers::Offset<bbfrc::msgs::PinInformation>> servoPinOffsets;

    makePinInfo(builder, digitalPinOffsets, digitalPins);
    makePinInfo(builder, analogPinOffsets, analogPins);
    makePinInfo(builder, servoPinOffsets, servoPins);

    auto digitalVec = builder.CreateVector(digitalPinOffsets);
    auto analogVec = builder.CreateVector(analogPinOffsets);
    auto servoVec = builder.CreateVector(servoPinOffsets);

    auto resp = bbfrc::msgs::CreateGetPinInformationResponse(builder, totalPins, digitalVec, analogVec, servoVec);
    return wrapMessage(builder, resp.Union(), bbfrc::msgs::Payload_GetPinInformationResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeGetFirmwareNameAndVersionRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize) {
    auto req = bbfrc::msgs::CreateGetFirmwareNameAndVersionRequest(builder);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_GetFirmwareNameAndVersionRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeGetFirmwareNameAndVersionResponse(flatbuffers::FlatBufferBuilder& builder, std::string name, uint8_t versionMajor, uint8_t versionMinor, int* bufferSize) {
    auto nameOffset = builder.CreateString(name);
    auto req = bbfrc::msgs::CreateGetFirmwareNameAndVersionResponse(builder, nameOffset, versionMajor, versionMinor);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_GetFirmwareNameAndVersionResponse, bufferSize);
}

uint8_t* FlatBuffersUtil::makeSetDigitalPinModeRequest(flatbuffers::FlatBufferBuilder& builder, uint8_t port, bbfrc::msgs::PinMode pinMode, int* bufferSize) {
    auto req = bbfrc::msgs::CreateSetDigitalPinModeRequest(builder, port, pinMode);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_SetDigitalPinModeRequest, bufferSize);
}

uint8_t* FlatBuffersUtil::makeSystemResetRequest(flatbuffers::FlatBufferBuilder& builder, int* bufferSize) {
    auto req = bbfrc::msgs::CreateSystemResetRequest(builder);
    return wrapMessage(builder, req.Union(), bbfrc::msgs::Payload_SystemResetRequest, bufferSize);
}


void makePinInfo(flatbuffers::FlatBufferBuilder& builder, std::vector<flatbuffers::Offset<bbfrc::msgs::PinInformation>>& pinInfoVector, std::vector<PinInformation> pinInfos) {
    for (auto it = pinInfos.begin(); it != pinInfos.end(); it++) {
        std::vector<int8_t> supportedModesVec;

        auto supportedModes = it->supportedModes;

        for (auto modeIt = supportedModes.begin(); modeIt != supportedModes.end(); modeIt++) {
            supportedModesVec.push_back(*modeIt);
        }

        auto supportedModesOffset = builder.CreateVector(supportedModesVec);
        auto pinInfo = bbfrc::msgs::CreatePinInformation(builder, it->physicalPin, it->mode, supportedModesOffset, it->analogChannel);
        pinInfoVector.push_back(pinInfo);
    }
}


uint8_t* FlatBuffersUtil::wrapMessage(flatbuffers::FlatBufferBuilder& builder, flatbuffers::Offset<void> payloadOffset, bbfrc::msgs::Payload type, int* bufferSize) {
    auto envelope = bbfrc::msgs::CreateEnvelope(builder, type, payloadOffset);
    builder.Finish(envelope);

    *bufferSize = builder.GetSize();
    return builder.GetBufferPointer();
}
