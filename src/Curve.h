//
// Created by fulva on 5/25/19.
//

#ifndef MOTOR_ENCODER_CURVE_H
#define MOTOR_ENCODER_CURVE_H


#include <stdint.h>

class Curve {
public:

    Curve(long initPos, long finalPos);

    virtual long getNowDest(long time) final;

    virtual long getNowDest(long time, long initPos, long finalPos) = 0;


private:
    long initPos;
    long finalPos;


};


#endif //MOTOR_ENCODER_CURVE_H
