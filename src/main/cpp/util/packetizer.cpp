#include "util/packetizer.h";

bool isHeader(std::vector<uint8_t>& buffer, int offset) {
    return buffer[offset] == 0xDE && buffer[offset+1] == 0xBE;
}

int readSize(std::vector<uint8_t>& buffer, int offset) {
    int msb = (int)buffer[offset];
    int lsb = (int)buffer[offset+1];

    return (msb << 8) + (lsb & 0xFF);
}

namespace drivethru {

    std::vector<uint8_t> Packetizer::makePacket(std::vector<uint8_t> payload) {
        std::vector<uint8_t> packet;
        packet.push_back(0xDE);
        packet.push_back(0xBE);

        // calculate the payload length
        uint32_t payloadSize = payload.size();
        uint8_t payloadSizeLSB = payloadSize & 0xFF;
        uint8_t payloadSizeMSB = (payloadSize >> 8) & 0xFF;
        packet.push_back(payloadSizeMSB);
        packet.push_back(payloadSizeLSB);

        // Shove the rest of the payload in here
        packet.insert(packet.end(), payload.begin(), payload.end());
        return packet;
    }

    void Packetizer::addChunk(std::vector<uint8_t> chunk) {
        // We have an empty buffer
        if (_buffer.empty() || _offset == _bufferLength) {
            _buffer.clear();
            // Copy the chunk values
            _buffer.insert(_buffer.end(), chunk.begin(), chunk.end());
            _bufferLength = chunk.size();
            _offset = 0;
            return;
        }

        int inChunkLength = chunk.size();
        int newLength = _bufferLength + inChunkLength;

        _buffer.insert(_buffer.end(), chunk.begin(), chunk.end());
        _bufferLength = newLength;
    }

    std::vector<uint8_t> Packetizer::read() {
        if (_bufferLength < 4 + _offset) {
            // We don't have enough bytes, so wait
            // Return an empty vector in the meantime
            return {};
        }

        // Make sure the header matches
        if (!isHeader(_buffer, _offset)) {
            // If it doesn't match, move the offset until we get to the pair
            while(_offset < _bufferLength - 1) {
                if (isHeader(_buffer, _offset)) {
                    break;
                }
                _offset++;
            }

            // If we don't have enough bytes in the buffer, return empty
            if (_bufferLength < 4 + _offset) {
                return {};
            }
        }

        int length = readSize(_buffer, _offset + 2);
        int remaining = _bufferLength - (_offset + 4);

        // We don't yet have a complete payload
        if (length > remaining) {
            return {};
        }

        _offset += 4;
        std::vector<uint8_t> result;
        result.insert(result.end(), _buffer.begin() + _offset, _buffer.begin() + _offset + length);
        _offset += length;
        return result;
    }
}
