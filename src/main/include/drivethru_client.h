#pragma once
#define ASIO_STANDALONE

#include <asio.hpp>
#include <memory>
#include <deque>

#include "util/packetizer.h"
#include "protocols/flatbuffers/drivethru_generated.h"

using asio::ip::tcp;

typedef std::function<void(const bbfrc::msgs::Envelope*)> PacketSubscriber;

class DrivethruClient {
public:
    DrivethruClient(asio::io_context& ctx);
    void AddPacketSubscriber(PacketSubscriber subscriber);

    void Write(const uint8_t* buffer, std::size_t size);

    void Connect(const tcp::resolver::results_type& endpoints);
    bool IsConnected();

private:
    void DoConnect(const tcp::resolver::results_type& endpoints);
    void DoReadData();
    void DoWrite();
    void BroadcastPacket(const bbfrc::msgs::Envelope* envelope);

private:
    asio::io_context& context_;
    tcp::socket socket_;
    std::vector<uint8_t> read_buffer_;
    std::vector<PacketSubscriber> subscribers_;
    drivethru::Packetizer packetizer_;

    bool connected_;

    std::deque<std::vector<uint8_t>> message_queue_;
};
