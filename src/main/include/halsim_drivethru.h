#pragma once

#include <vector>
#include "drivethru_node.h"

class DrivethruDIO;
class DrivethruPWM;
class DrivethruAnalogIn;

class HALSimDrivethru {
public:
    static const int kPwmCount = 10;
    static const int kAnalogInCount = 6;

    DrivethruNode node;

    std::vector<DrivethruPWM*> pwms;
    std::vector<DrivethruDIO*> dios;
    std::vector<DrivethruAnalogIn*> analogios;
};
