#include "drivethru_dio.h"

#include <iostream>

#include "hal/DIO.h"
#include <mockdata/HAL_Value.h>
#include <mockdata/DIOData.h>
#include <mockdata/NotifyListener.h>

static void init_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);
    dio->SetInitialized(value->data.v_boolean);
    dio->setIsInput(true);
    dio->setListeningMode(dio->IsInitialized());
}

// Only seems to be called for Digital writes, so value is 0
static void isinput_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);
    bool isInput = value->data.v_boolean;
    // Reset Listening Mode now that is input is known
    dio->setIsInput(isInput);
    dio->setListeningMode(isInput);
                          
}

static void setvalue_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruDIO* dio = static_cast<DrivethruDIO*>(param);
    // Because HALSIM_SetDIOValue triggers setValue callback, 
    // we need to protect from retriggering to drivethru
    if( !dio->IsInput() ) {
      dio->publishValue(value->data.v_boolean);
    }
}

DrivethruDIO::DrivethruDIO(int port, HALSimDrivethru* halsim) 
    : port_(port),
    halsim_(halsim),
    initialized_(false),
    has_listener_(false),
    is_input_(true)
{
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

void DrivethruDIO::setIsInput(bool value) {
    is_input_ = value;
}

bool DrivethruDIO::IsInput() {
    return is_input_;
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

    if(has_listener_) {
        has_listener_ = false;
        halsim_->node.RemoveDigitalInputListener(port_);
    }
}

void DrivethruDIO::Callback(bool value) {
    // From SimDataValue.h, HAL_SIMDATAVALUE_DEFINE_CAPI Set##CAPINAME
    // should only set the data value. But this somehow triggers a set value callback.
    HALSIM_SetDIOValue(port_, value);
}
