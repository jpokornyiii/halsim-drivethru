#include "drivethru_dio.h"

#include <iostream>

#include <mockdata/HAL_Value.h>
#include <mockdata/DIOData.h>
#include <mockdata/NotifyListener.h>

static void init_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);
    dio->SetInitialized(value->data.v_boolean);
    // TODO: Unable to distinguish between reads and writes at this time. Assume digitial read.
    dio->setListeningMode(dio->IsInitialized());
}

// Only seems to be called for Digital writes, so value is 0
static void isinput_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);
    bool isInput = value->data.v_boolean;
    // Reset Listening Mode now that is input is known
    dio->setListeningMode(isInput);
                          
}

static void setvalue_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);    
    dio->publishValue(value->data.v_boolean);
}

DrivethruDIO::DrivethruDIO(int port, HALSimDrivethru* halsim) {
    port_ = port;
    halsim_ = halsim;
    HALSIM_RegisterDIOInitializedCallback(port, init_callback, this, false);
    HALSIM_RegisterDIOIsInputCallback(port, isinput_callback, this, false);
    HALSIM_RegisterDIOValueCallback(port, setvalue_callback, this, false);
}

void DrivethruDIO::SetInitialized(bool value) {
    initialized_ = value;
}

bool DrivethruDIO::IsInitialized() {
    return initialized_;
}

int DrivethruDIO::getPort() {
    return port_;
}

void DrivethruDIO::setListeningMode(bool isListening)
{
    if(isListening) {
        Listen();
    } else {
        StopListening();
    }
}

void DrivethruDIO::publishValue(bool value) {
    halsim_->node.PublishDigital(port_, value);
}

void DrivethruDIO::Listen() {
    if (!has_listener_) {
        has_listener_ = true;
        halsim_->node.AddDigitalInputListener(port_, [this](int port, bool value) {
            Callback(value);
        });
    }
}

void DrivethruDIO::StopListening() {
    //if(has_listener_) {
    //    has_listener_ = false;
        // TODO: Remove listener from node object.
        // Something like halsim_->node.RemoveDigitalInputListner(port_);
    //}
}

void DrivethruDIO::Callback(bool value) {
    HALSIM_SetDIOValue(port_, value);
}

