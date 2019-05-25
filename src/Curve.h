//
// Created by fulva on 5/25/19.
//

#ifndef MOTOR_ENCODER_CURVE_H
#define MOTOR_ENCODER_CURVE_H


#include <stdint.h>

class Curve {
public:
    virtual long getNowDest(long time) = 0;


};


#endif //MOTOR_ENCODER_CURVE_H
