//
// Created by fulva on 2019/3/29.
//

#ifndef ARDUINOSAMPLE_MOTOR_H
#define ARDUINOSAMPLE_MOTOR_H


#include <stdint-gcc.h>
#include <MegaEncoderCounter.h>
#include "ResetResponse.h"

class Motor {
public:
    enum Mode {
        GO, OSCILLATE
    };

    Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis,
          uint8_t homePin);

    void drive(long speed);

    void goTo(long destination);

    void findHome();

    void resetAxis();

    void activate();

    int getAxis();

    void setFirstDest(int dest);

    void setSecondDest(int dest);

private:
    Mode mode = GO;
public:
    void setMode(Mode mode);

private:
    int destinations[2]{0, 0};
    char axis;
    MegaEncoderCounter &megaEncoderCounter;
    uint8_t pin1;
    uint8_t pin2;
    uint8_t pwm_e;
    ResetResponse resetResponse;
};


#endif //ARDUINOSAMPLE_MOTOR_H
