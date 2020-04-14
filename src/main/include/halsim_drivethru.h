#pragma once

#include <vector>
#include "drivethru_node.h"

class DrivethruDIO;
class DrivethruPWM;

class HALSimDrivethru {
public:
    static const int kPwmCount = 10;

    DrivethruNode node;

    std::vector<DrivethruPWM*> pwms;
    std::vector<DrivethruDIO*> dios;
};
