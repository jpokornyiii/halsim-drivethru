/**
 * PURELY FOR DEMO TESTING ONLY
 * PLEASE REMEMBER TO REMOVE THIS FILE AFTER YOU HAVE YOUR OWN IMPLEMENTATION
 */
#include "PrintPWM.h"

#include <iostream>

// NOTE: at some point in 2020, mockdata/HAL_Value.h moves to hal/Value.h
#include <mockdata/HAL_Value.h>
#include <mockdata/NotifyListener.h>
#include <mockdata/PWMData.h>

#include <flatbuffers/flatbuffers.h>
#include "protocols/flatbuffers/drivethru_generated.h"

static void PWMCallback(const char* name, void* param,
                        const struct HAL_Value* value) {
    auto pwm = static_cast<PrintPWM*>(param);
    pwm->Publish(value->data.v_double);
}

PrintPWM::PrintPWM(int port) {
    m_port = port;
    HALSIM_RegisterPWMSpeedCallback(port, PWMCallback, this, false);
}

void PrintPWM::Publish(double value) {
    std::cout << "PWM " << m_port << ": " << value << std::endl;
}
