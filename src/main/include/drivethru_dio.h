#pragma once

#include "halsim_drivethru.h"

class DrivethruDIO {
public:
    DrivethruDIO(int port, HALSimDrivethru* halsim);
    void SetInitialized(bool value);
    bool IsInitialized();
    void publishValue(bool value);
    int getPort();
    void setListeningMode(bool isListening);

private:
    HALSimDrivethru* halsim_;
    int port_;
    bool initialized_;

    void Listen();
    void StopListening();
    void Callback(bool value);

    bool has_listener_;
};
