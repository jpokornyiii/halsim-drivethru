#include "drivethru_client.h"
#include <iostream>

DrivethruClient::DrivethruClient(asio::io_context& ctx)
    : context_(ctx),
      socket_(ctx),
      read_buffer_(256),
      connected_(false) {

    // no-op
}

void DrivethruClient::AddPacketSubscriber(PacketSubscriber subscriber) {
    subscribers_.push_back(subscriber);
}

void DrivethruClient::Write(const uint8_t* buffer, std::size_t size) {
    std::vector<uint8_t> buf_vec(buffer, buffer + size);
    auto packet = drivethru::Packetizer::makePacket(buf_vec);
    asio::post(context_,
        [this, packet]() {
            bool write_in_progress = !message_queue_.empty();
            message_queue_.push_back(packet);
            if (!write_in_progress) {
                DoWrite();
            }
        });
}

void DrivethruClient::WriteSync(const uint8_t* buffer, std::size_t size) {
    std::size_t write_len = asio::write(socket_, asio::buffer(buffer, size));
    if( write_len != size) {
        std::cerr << " Write size mismatch. Buffer size " << size << " actual size " << write_len << std::endl;
    }
}

void DrivethruClient::Connect(const tcp::resolver::results_type& endpoints) {
    DoConnect(endpoints);
}

bool DrivethruClient::IsConnected() {
    return connected_;
}

void DrivethruClient::DoConnect(const tcp::resolver::results_type& endpoints) {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint) {
            if (!ec) {
                // TODO broadcast to listener?
                connected_ = true;

                DoReadData();
            }
        });
        // TODO do we want some sort of exponential backoff retry?
}

/**
 * Read more data
 * This is the meat of the client, which listens for messages
 * from the server and broadcasts available packets to listeners
 */
void DrivethruClient::DoReadData() {
    // We use async_read_some (on the socket) because we don't care
    // about delimiters, and will let the packetizer handle this
    // for us
    socket_.async_read_some(asio::buffer(read_buffer_),
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                packetizer_.addChunk(
                    std::vector<uint8_t>(read_buffer_.begin(),
                                         read_buffer_.begin() + length));

                std::vector<uint8_t> buf = packetizer_.read();
                while (buf.size() > 0) {
                    auto env_ptr = bbfrc::msgs::GetEnvelope(buf.data());
                    BroadcastPacket(env_ptr);
                    buf = packetizer_.read();
                }

                // Come around for another go at reading
                // It's async, so this basically puts some work
                // on the io_context queue and returns
                DoReadData();
            }
            // TODO Handle error codes
        });
}

/**
 * Write whatever is in the queue right now
 */
void DrivethruClient::DoWrite() {
    asio::async_write(socket_,
        asio::buffer(message_queue_.front()),
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                message_queue_.pop_front();
                if (!message_queue_.empty()) {
                    DoWrite();
                }
            }
        });
}

void DrivethruClient::BroadcastPacket(const bbfrc::msgs::Envelope* envelope) {
    for (auto it = subscribers_.begin(); it != subscribers_.end(); it++) {
        (*it)(envelope);
    }
}
