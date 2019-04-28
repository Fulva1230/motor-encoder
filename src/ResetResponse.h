//
// Created by fulva on 4/26/19.
//

#ifndef MOTOR_ENCODER_RESETRESPONSE_H
#define MOTOR_ENCODER_RESETRESPONSE_H


#include <stdint.h>

class ResetResponse {
public:
    ResetResponse(uint8_t homePin);

    bool isActivate();

private:
    uint8_t homePin;

};


#endif //MOTOR_ENCODER_RESETRESPONSE_H
