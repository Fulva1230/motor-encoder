//
// Created by fulva on 6/14/19.
//

#ifndef MOTOR_ENCODER_ACURVE_H
#define MOTOR_ENCODER_ACURVE_H


#include "Curve.h"

class aCurve : public Curve {
public:
    aCurve(long initPos, long finalPos, uint16_t ta, uint16_t totalTime);

    long getNowDest(long time, long initPos, long finalPos) override;

private:
    uint16_t ta;
    uint16_t totalTime;

};


#endif //MOTOR_ENCODER_ACURVE_H
