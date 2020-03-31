#ifndef DRIVETHRU_PACKETIZER_H
#define DRIVETHRU_PACKETIZER_H

#include <cstdlib>
#include <vector>

namespace drivethru {

    class Packetizer {
        public:
            static std::vector<uint8_t> makePacket(std::vector<uint8_t> payload);
            void addChunk(std::vector<uint8_t> chunk);
            std::vector<uint8_t> read();

        private:
            std::vector<uint8_t> _buffer;
            uint32_t _bufferLength = 0;
            uint32_t _offset = 0;
    };

}

bool isHeader(std::vector<uint8_t>& buffer, int offset);
int readSize(std::vector<uint8_t>& buffer, int offset);

#endif
