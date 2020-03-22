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

    return 0;
}

} // extern "C"
