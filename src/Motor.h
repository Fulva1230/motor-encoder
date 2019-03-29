//
// Created by fulva on 2019/3/29.
//

#ifndef ARDUINOSAMPLE_MOTOR_H
#define ARDUINOSAMPLE_MOTOR_H


#include <stdint-gcc.h>
#include <MegaEncoderCounter.h>

class Motor {
public:

    Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis);

    void drive(long speed);

    void goTo(long destination);

    void resetAxis();

    void activate();

private:
    char axis;
    MegaEncoderCounter &megaEncoderCounter;
    uint8_t pin1;
    uint8_t pin2;
    uint8_t pwm_e;
    int destination;
};


#endif //ARDUINOSAMPLE_MOTOR_H
