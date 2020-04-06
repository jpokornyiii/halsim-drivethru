#pragma once

#include "halsim_drivethru.h"

class DrivethruPWM {
public:
    DrivethruPWM(int port, HALSimDrivethru* halsim);
    void SetInitialized(bool value) { initialized_ = value; }
    bool IsInitialized() { return initialized_; }
    void Publish(double value);

private:
    HALSimDrivethru* halsim_;
    bool initialized_;
    int port_;
};
