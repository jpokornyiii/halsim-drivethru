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
    // Unsubscribe from each digital input pin
    // TODO: This is not working. Message is not received by drivethru
    for( auto it = digital_callbacks_map_.begin(); it != digital_callbacks_map_.end();it++)  {
        SendDigitalSubscriptionRequest(it->first, false, true);
    }
    for( auto it = analog_callbacks_map_.begin(); it != analog_callbacks_map_.end(); it++) {
        SendAnalogSubscriptionRequest(it->first, false, true);
    }
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
        SendDigitalSubscriptionRequest(port, true);
        digital_callbacks_map_.insert(
            std::pair<int, std::vector<OnDigitalInputChangedCallback>>(port, { cb }));
    }
    else {
        it->second.push_back(cb);
    }
}

void DrivethruNode::RemoveDigitalInputListener(int port) {
    auto it = digital_callbacks_map_.find(port);
    if( it != digital_callbacks_map_.end()) 
    {
        SendDigitalSubscriptionRequest(port, false);
        digital_callbacks_map_.erase(it);
    }
}

void DrivethruNode::AddAnalogInputListener(int port, OnAnalogInputChangedCallback cb) {
    std::cout << " Adding analog input listener for port " << port << std::endl;
    auto it = analog_callbacks_map_.find(port);
    if (it == analog_callbacks_map_.end()) {
        SendAnalogSubscriptionRequest(port, true);
        analog_callbacks_map_.insert(
            std::pair<int, std::vector<OnAnalogInputChangedCallback>>(port, { cb }));
    }
    else {
        it->second.push_back(cb);
    }
}

void DrivethruNode::RemoveAnalogInputListener(int port) {
    auto it = analog_callbacks_map_.find(port);
    if( it != analog_callbacks_map_.end()) 
    {
        SendAnalogSubscriptionRequest(port, false);
        analog_callbacks_map_.erase(it);
    }
}

// API
FirmwareInfo DrivethruNode::GetFirmwareInfo() {
    FirmwareInfo ret(firmware_info_);
    return ret;
}

void DrivethruNode::PublishDigital(int port, bool value) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeDigitalWriteRequest(buffer_builder_, port, value, &buf_size);
    client_.Write(buf, buf_size);
}

void DrivethruNode::PublishServoAngle(int port, int angle) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeSetServoAngleRequest(buffer_builder_, port, angle, &buf_size);
    client_.Write(buf, buf_size);
}

void DrivethruNode::OnPacketReceived(const bbfrc::msgs::Envelope* envelope) {
    switch (envelope->payload_type()) {
        case bbfrc::msgs::Payload_GetFirmwareNameAndVersionResponse: {
            has_firmware_info_ = true;
            auto payload = envelope->payload_as_GetFirmwareNameAndVersionResponse();
            firmware_info_.name.assign(payload->name()->c_str());
            firmware_info_.version_major = payload->versionMajor();
            firmware_info_.version_minor = payload->versionMinor();
            
            std::cout << "Drivethru firmware: " << firmware_info_.name 
                      << ":" << firmware_info_.version_major
                      << "." << firmware_info_.version_minor 
                      << std::endl;

            if (waiting_for_firmware_) {
                waiting_for_firmware_ = false;
                BroadcastOnConnected(GetFirmwareInfo());
            }
        } break;
        case bbfrc::msgs::Payload_DigitalReadSubscribeResponse: {
            auto payload = envelope->payload_as_DigitalReadSubscribeResponse();
            // TODO: add port in response
            // std::cout << "Subscription to port: " << payload->port() << " success: " << payload->success() << std::endl;
        } break;
        case bbfrc::msgs::Payload_DigitalReadResponse: {
            auto payload = envelope->payload_as_DigitalReadResponse();
            BroadcastDigital(payload->port(), payload->value());
        } break;
        case bbfrc::msgs::Payload_AnalogReadResponse: {
            auto payload = envelope->payload_as_AnalogReadResponse();
            BroadcastAnalog(payload->port(), payload->value());
        } break;
        case bbfrc::msgs::Payload_AnalogReadSubscribeResponse: {
            auto payload = envelope->payload_as_AnalogReadSubscribeResponse();
            // TODO: add port in response
            //std::cout << "Subscription to port: " << payload->port() << " success: " << payload->success() << std::endl;
        } break;
        // TODO Implement other response handlers
        default: {
            const char * payload_name = bbfrc::msgs::EnumNamePayload(envelope->payload_type());
            std::cout << "Unknown message type: " << payload_name << std::endl;
        }
    }
}

void DrivethruNode::SendFirmwareRequest() {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeGetFirmwareNameAndVersionRequest(buffer_builder_, &buf_size);
    client_.Write(buf, buf_size);
}

void DrivethruNode::SendDigitalSubscriptionRequest(int port, bool subscribe, bool is_sync) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeDigitalReadSubscribeRequest(buffer_builder_, port, subscribe, &buf_size);
    if( is_sync ) {
        client_.WriteSync(buf, buf_size);
    } else {
        client_.Write(buf, buf_size);
    }
}

void DrivethruNode::SendAnalogSubscriptionRequest(int port, bool subscribe, bool is_sync) {
    buffer_builder_.Reset();
    int buf_size;
    uint8_t* buf = FlatBuffersUtil::makeAnalogReadSubscribeRequest(buffer_builder_, port, subscribe, &buf_size);
    if( is_sync ) {
        client_.WriteSync(buf, buf_size);
    } else {
        client_.Write(buf, buf_size);
    }
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
