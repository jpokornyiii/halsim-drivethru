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
    if (dio->IsInitialized()) {
        dio->Listen();
    }
}

DrivethruDIO::DrivethruDIO(int port, HALSimDrivethru* halsim) {
    port_ = port;
    halsim_ = halsim;
    HALSIM_RegisterDIOInitializedCallback(port, init_callback, this, true);

    // TODO Investigate if the line below gets called when we do
    // .set() on a DigitalOutput
    // HALSIM_RegisterDIOValueCallback
}

void DrivethruDIO::SetInitialized(bool value) {
    initialized_ = value;
}

bool DrivethruDIO::IsInitialized() {
    return initialized_;
}

void DrivethruDIO::Listen() {
    if (!has_listener_) {
        has_listener_ = true;
        halsim_->node.AddDigitalInputListener(port_, [this](int port, bool value) {
            Callback(value);
        });
    }
}

void DrivethruDIO::Callback(bool value) {
    HALSIM_SetDIOValue(port_, value);
}
