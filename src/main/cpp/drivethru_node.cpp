

#include "drivethru_node.h"

#include <iostream>
#include "protocols/flatbuffers/flatbuffer_util.h"

DrivethruNode::DrivethruNode()
    : io_context_(),
      client_(io_context_),
      run_thread_(nullptr),
      has_firmware_info_(false),
      buffer_builder_(512) {

    firmware_info_ = { "", 0, 0 };

    // Add client packet listener
    client_.AddPacketSubscriber(
        std::bind(&DrivethruNode::OnPacketReceived,
                  this,
                  std::placeholders::_1));
}

DrivethruNode::~DrivethruNode() {
    Stop();
}

bool DrivethruNode::Connect(std::string host, int port) {
    return Connect(host, std::to_string(port));
}

bool DrivethruNode::Connect(std::string host, std::string service) {
    if (client_.IsConnected()) {
        return true;
    }

    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host, service);
    client_.Connect(endpoints);

    waiting_for_firmware_ = true;

    // Make a request for firmware version
    SendFirmwareRequest();

    // Start the run thread
    run_thread_ = new std::thread([this]() { io_context_.run(); });

    return true;
}

void DrivethruNode::Stop() {
    if (run_thread_ != nullptr) {
        run_thread_->join();
        delete run_thread_;
        run_thread_ = nullptr;
    }
}

bool DrivethruNode::IsConnected() {
    return client_.IsConnected() && has_firmware_info_;
}

// Subscriptions
void DrivethruNode::AddOnConnectedListener(OnConnectedCallback cb) {
    onconnected_callbacks_.push_back(cb);
}

void DrivethruNode::AddDigitalInputListener(int port, OnDigitalInputChangedCallback cb) {
    auto it = digital_callbacks_map_.find(port);
    if (it == digital_callbacks_map_.end()) {
        SendDigitalSubscriptionRequest(port);
        digital_callbacks_map_.insert(
            std::pair<int, std::vector<OnDigitalInputChangedCallback>>(port, { cb }));
    }
    else {
        it->second.push_back(cb);
    }
}

void DrivethruNode::AddAnalogInputListener(int port, OnAnalogInputChangedCallback cb) {
    auto it = analog_callbacks_map_.find(port);
    if (it == analog_callbacks_map_.end()) {
        SendAnalogSubscriptionRequest(port);
        analog_callbacks_map_.insert(
            std::pair<int, std::vector<OnAnalogInputChangedCallback>>(port, { cb }));
    }
    else {
        it->second.push_back(cb);
    }
}

// API
FirmwareInfo DrivethruNode::GetFirmwareInfo() {
    FirmwareInfo ret(firmware_info_);
    return ret;
}

void DrivethruNode::OnPacketReceived(const bbfrc::msgs::Envelope* envelope) {
    switch (envelope->payload_type()) {
        case bbfrc::msgs::Payload_GetFirmwareNameAndVersionResponse: {
            has_firmware_info_ = true;
            auto payload = envelope->payload_as_GetFirmwareNameAndVersionResponse();
            firmware_info_.name.assign(payload->name()->c_str());
            firmware_info_.version_major = payload->versionMajor();
            firmware_info_.version_minor = payload->versionMinor();

            if (waiting_for_firmware_) {
                waiting_for_firmware_ = false;
                BroadcastOnConnected(GetFirmwareInfo());
            }
        } break;

        case bbfrc::msgs::Payload_DigitalReadResponse: {
            auto payload = envelope->payload_as_DigitalReadResponse();
            BroadcastDigital(payload->port(), payload->value());
        } break;

        // TODO Implement other response handlers

        default:
            std::cout << "Unknown message type" << std::endl;
    }
}

void DrivethruNode::SendFirmwareRequest() {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeGetFirmwareNameAndVersionRequest(buffer_builder_, &buf_size);
    client_.Write(buf, buf_size);
}

void DrivethruNode::SendDigitalSubscriptionRequest(int port) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeDigitalReadSubscribeRequest(buffer_builder_, port, true, &buf_size);
    client_.Write(buf, buf_size);
}

void DrivethruNode::SendAnalogSubscriptionRequest(int port) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeAnalogReadSubscribeRequest(buffer_builder_, port, true, &buf_size);
    client_.Write(buf, buf_size);
}

// Broadcast methods
void DrivethruNode::BroadcastOnConnected(FirmwareInfo fw) {
    for (auto it = onconnected_callbacks_.begin(); it != onconnected_callbacks_.end(); it++) {
        (*it)(fw);
    }
}

void DrivethruNode::BroadcastDigital(int port, bool value) {
    auto subscribers = digital_callbacks_map_.find(port);
    if (subscribers != digital_callbacks_map_.end()) {
        auto subscriber_list = subscribers->second;
        for (auto it = subscriber_list.begin(); it != subscriber_list.end(); it++) {
            (*it)(port, value);
        }
    }
}

void DrivethruNode::BroadcastAnalog(int port, int value) {
    auto subscribers = analog_callbacks_map_.find(port);
    if (subscribers != analog_callbacks_map_.end()) {
        auto subscriber_list = subscribers->second;
        for (auto it = subscriber_list.begin(); it != subscriber_list.end(); it++) {
            (*it)(port, value);
        }
    }
}
