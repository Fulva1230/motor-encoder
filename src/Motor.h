//
// Created by fulva on 2019/3/29.
//

#ifndef ARDUINOSAMPLE_MOTOR_H
#define ARDUINOSAMPLE_MOTOR_H


#include <stdint-gcc.h>
#include <MegaEncoderCounter.h>

class Motor {
public:
    enum Mode {
        GO, OSCILLATE
    };

    Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis);

    void drive(long speed);

    void goTo(long destination);

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
};


#endif //ARDUINOSAMPLE_MOTOR_H
