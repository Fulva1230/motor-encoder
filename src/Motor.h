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
        GO, OSCILLATE, STOP, HOME
    };

    Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis, uint8_t homePin,
          double angToCou, uint8_t maxSpeed);

    void drive(long speed);

    void goTo(long destination);

    void driveAngle(long desAng);

    void resetAxis();

    void activate();

    int getAxis();

    void setFirstDest(int dest);

    void setSecondDest(int dest);

    Mode getMode();

    uint8_t getHomePin() const;

    boolean getNeedAttach() const;

    void setNeedAttach(boolean needAttach);

    void setMode(Mode mode);

private:
    Mode mode{STOP};

private:
    int destinations[2]{0, 0};
    char axis;
    MegaEncoderCounter &megaEncoderCounter;
    uint8_t pin1;
    uint8_t pin2;
    uint8_t pwm_e;
    uint8_t homePin;
    boolean needAttach{true};
    double angleToCount;
    const uint8_t maxSpeed;
    ResetResponse resetResponse;
};


#endif //ARDUINOSAMPLE_MOTOR_H
