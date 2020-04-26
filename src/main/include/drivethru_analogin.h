#pragma once

#include "halsim_drivethru.h"

class DrivethruAnalogIn {
public:
    DrivethruAnalogIn(int port, HALSimDrivethru* halsim);
    void SetInitialized(bool value) { initialized_ = value; }
    bool IsInitialized() { return initialized_; }
    void Listen();
    void StopListening();
    int getPort();
    
private:
    void Callback(int value);

    HALSimDrivethru* halsim_;
    bool initialized_;
    int port_;
    bool has_listener_;
};
