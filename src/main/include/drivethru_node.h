#pragma once

#define ASIO_STANDALONE

#include <asio.hpp>
#include <thread>
#include <map>

#include "drivethru_client.h"

typedef struct FirmwareInfo {
    std::string name;
    unsigned int version_major;
    unsigned int version_minor;
} FirmwareInfo;

// Callback signatures
typedef std::function<void(FirmwareInfo)> OnConnectedCallback;
typedef std::function<void(int, bool)> OnDigitalInputChangedCallback;
typedef std::function<void(int, int)> OnAnalogInputChangedCallback;

class DrivethruNode {
public:
    DrivethruNode();
    ~DrivethruNode();

    bool Connect(std::string host, int port);
    bool Connect(std::string host, std::string port);
    void Stop();

    bool IsConnected();

    // Subscriptions
    void AddOnConnectedListener(OnConnectedCallback callback);
    void AddDigitalInputListener(int port, OnDigitalInputChangedCallback callback);
    void AddAnalogInputListener(int port, OnAnalogInputChangedCallback callback);

    // API
    FirmwareInfo GetFirmwareInfo();

private:
    void OnPacketReceived(const bbfrc::msgs::Envelope* envelope);

    // Convenience functions for sending packets
    void SendFirmwareRequest();
    void SendDigitalSubscriptionRequest(int port);
    void SendAnalogSubscriptionRequest(int port);

    // Broadcast
    void BroadcastOnConnected(FirmwareInfo fw_info);
    void BroadcastDigital(int port, bool value);
    void BroadcastAnalog(int port, int value);

private:
    asio::io_context io_context_;
    DrivethruClient client_;
    std::thread* run_thread_;

    flatbuffers::FlatBufferBuilder buffer_builder_;

    bool waiting_for_firmware_;
    bool has_firmware_info_;
    FirmwareInfo firmware_info_;

    // Subscriptions
    std::vector<OnConnectedCallback> onconnected_callbacks_;
    std::map<int, std::vector<OnDigitalInputChangedCallback>> digital_callbacks_map_;
    std::map<int, std::vector<OnAnalogInputChangedCallback>> analog_callbacks_map_;
};
