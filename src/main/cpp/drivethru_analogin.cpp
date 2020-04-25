#include "drivethru_analogin.h"

#include <string>
#include <iostream>

#include <mockdata/HAL_Value.h>
#include <mockdata/NotifyListener.h>
#include <mockdata/AnalogInData.h>

static void init_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruAnalogIn* analogIn = static_cast<DrivethruAnalogIn*>(param);
    // If initializing, add listener
    analogIn->SetInitialized(value->data.v_boolean);
    if(analogIn->IsInitialized())
        analogIn->Listen();
}

DrivethruAnalogIn::DrivethruAnalogIn(int port, HALSimDrivethru* halsim) :
port_(port),
halsim_(halsim),
has_listener_(false)
{
    HALSIM_RegisterAnalogInInitializedCallback(port, init_callback, this, false);
}

int DrivethruAnalogIn::getPort()
{
    return port_;
}

void DrivethruAnalogIn::Listen() {
    
    if (!has_listener_) {
        has_listener_ = true;
    
        halsim_->node.AddAnalogInputListener(port_, [this](int port, int value) {
            Callback(value);
        });
    }
}

void DrivethruAnalogIn::StopListening() {

    if(has_listener_) {
        has_listener_ = false;
        halsim_->node.RemoveAnalogInputListener(port_);
    }
}

void DrivethruAnalogIn::Callback(int value) {
    /* Value in is from 0 - 1023 from the A2D on an Arduino. 
       This may need to be configurable in the future. 
       Need to transform this to 0V-5V
    */
    // Need to use const double to ensure operators are using floating point precision
    const double HIGH_INPUT_VALUE = 1023.0;
    const double HIGH_INPUT_VOLTAGE = 5.0;
    double voltage = (value/HIGH_INPUT_VALUE) * HIGH_INPUT_VOLTAGE;
    HALSIM_SetAnalogInVoltage(port_, voltage);
}
