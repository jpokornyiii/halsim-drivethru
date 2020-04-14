#include "drivethru_pwm.h"

#include <string>

#include <mockdata/HAL_Value.h>
#include <mockdata/NotifyListener.h>
#include <mockdata/PWMData.h>

static void init_callback(const char* name,
                          void* param,
                          const struct HAL_Value* value) {
    DrivethruPWM* pwm = static_cast<DrivethruPWM*>(param);
    pwm->SetInitialized(value->data.v_boolean);
}

static void speed_callback(const char* name,
                           void* param,
                           const struct HAL_Value* value) {
    DrivethruPWM* pwm = static_cast<DrivethruPWM*>(param);
    if (pwm->IsInitialized()) {
        pwm->Publish(value->data.v_double);
    }
}

DrivethruPWM::DrivethruPWM(int port, HALSimDrivethru* halsim) {
    port_ = port;
    halsim_ = halsim;
    HALSIM_RegisterPWMInitializedCallback(port, init_callback, this, true);
    HALSIM_RegisterPWMSpeedCallback(port, speed_callback, this, true);
}

void DrivethruPWM::Publish(double value) {
    // convert [-1.0, 1.0] to [0, 180]
    if (value < -1.0) value = -1.0;
    if (value > 1.0) value = 1.0;

    int angle = (int)(((value + 1.0) / 2.0) * 180.0);
    halsim_->node.PublishServoAngle(port_, angle);
}
