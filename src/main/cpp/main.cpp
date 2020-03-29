/**
 * PURELY FOR DEMO TESTING ONLY
 * PLEASE REMEMBER TO REMOVE THIS FILE AFTER YOU HAVE YOUR OWN IMPLEMENTATION
 */
#define ASIO_STANDALONE

#include <iostream>

#include <hal/Ports.h>

#include "HALSimPrint.h"
#include "PrintPWM.h"

#include <asio.hpp>

#include "protocols/flatbuffers/flatbuffer_util.h"

static HALSimPrint halsim;

extern "C" {
#if defined(WIN32) || defined(_WIN32)
__declspec(dllexport)
#endif

int HALSIM_InitExtension(void) {
    std::cout << "Drivethru Print Simulator initializing..." << std::endl;

    int pwmCount = HAL_GetNumPWMChannels();
    halsim.m_pwms.reserve(pwmCount);
    for (int i = 0; i < pwmCount; i++) {
        halsim.m_pwms.emplace_back(i);
    }

    // EXAMPE CODE FOR TESTING
    // The following shows how to generate a buffer suitable for packetization
    // and sent over the network

    // Instantiate a FlatBufferBuilder
    flatbuffers::FlatBufferBuilder builder(256);

    // Each of the make* functions in FlatBuffersUtil returns a pointer to
    // the start of the buffer. Each also takes in a size pointer where the
    // buffer size will be written.
    // Each of the make* functions also takes in a reference to the FlatBufferBuilder
    // and other appropriate data
    int size = -1;
    uint8_t* buf = FlatBuffersUtil::makeSetServoAngleRequest(builder, 1, 125, &size);

    // At this point, buf points to the start of the generated buffer and
    // size contains the size of said buffer


    // Reading from a buffer
    // Assuming we have a buffer pointer, we can get a reference to the Envelope
    auto envelope = bbfrc::msgs::GetEnvelope(buf);

    // here, envelope->payload_type() tells us what the type of message the payload is


    return 0;
}

} // extern "C"
