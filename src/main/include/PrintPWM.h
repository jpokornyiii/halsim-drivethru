/**
 * PURELY FOR DEMO TESTING ONLY
 * PLEASE REMEMBER TO REMOVE THIS FILE AFTER YOU HAVE YOUR OWN IMPLEMENTATION
 */
#pragma once

#include "HALSimPrint.h"

class PrintPWM {
    public:
        explicit PrintPWM(int port);
        void Publish(double value);

    private:
        int m_port;
};