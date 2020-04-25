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
    void setIsInput(bool value);
    bool IsInput();

private:
    void Listen();
    void StopListening();
    void Callback(bool value);

    HALSimDrivethru* halsim_;
    int port_;
    bool initialized_;
    bool is_input_;
    bool has_listener_;
};
