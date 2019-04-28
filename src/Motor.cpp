//
// Created by fulva on 2019/3/29.
//

#include <HID.h>
#include "Motor.h"

#define MAX_SPEED 100

Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis,
             uint8_t homePin) : pin1(pin1),
                                pin2(pin2),
                                pwm_e(pwmE),
                                megaEncoderCounter(megaEncoderCounter1),
                                axis(axis),
                                resetResponse(homePin),
                                homePin(homePin) {}

void Motor::drive(long speed) {
    if (speed > 0) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        if (speed > MAX_SPEED) {
            analogWrite(pwm_e, MAX_SPEED);
        } else {
            analogWrite(pwm_e, speed);
        }
    } else {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        if (abs(speed) > MAX_SPEED) {
            analogWrite(pwm_e, MAX_SPEED);
        } else {
            analogWrite(pwm_e, abs(speed));
        }

    }
}

void Motor::goTo(long destination) {
    mode = GO;
    this->destinations[0] = destination;
}

void Motor::activate() {
    int position(getAxis());
    switch (mode) {
        case OSCILLATE:
            if (abs(destinations[0] - position) <= 50) {
                int swap;
                swap = destinations[0];
                destinations[0] = destinations[1];
                destinations[1] = swap;
            }
            drive(destinations[0] - position);
            Serial.print("run left:");
            Serial.println(abs(destinations[0] - position));
            break;
        case GO:
            if (abs(destinations[0] - position) > 50) {
                drive(destinations[0] - position);
                Serial.print("run left:");
                Serial.println(abs(destinations[0] - position));
            } else {
                drive(0);
                mode = STOP;
            }
            break;
        case HOME:
            drive(100);
            if (resetResponse.isActivate()) {
                drive(0);
                mode = STOP;
                resetAxis();
                goTo(0);
            }
        case STOP:
            drive(0);
    }
}

void Motor::resetAxis() {
    switch (axis) {
        case 'x':
            megaEncoderCounter.XAxisReset();
            break;
        case 'y':
            megaEncoderCounter.YAxisReset();
            break;
    }
}

int Motor::getAxis() {
    switch (axis) {
        case 'x':
            return megaEncoderCounter.XAxisGetCount() > INT32_MAX ? megaEncoderCounter.XAxisGetCount() - UINT32_MAX - 1
                                                                  : megaEncoderCounter.XAxisGetCount();
        case 'y':
            return megaEncoderCounter.YAxisGetCount() > INT32_MAX ? megaEncoderCounter.YAxisGetCount() - UINT32_MAX - 1
                                                                  : megaEncoderCounter.YAxisGetCount();
    }
    return 0;
}

void Motor::setMode(Motor::Mode mode) {
    Motor::mode = mode;
}

void Motor::setFirstDest(int dest) {
    this->destinations[0] = dest;
}

void Motor::setSecondDest(int dest) {
    this->destinations[1] = dest;
}

Motor::Mode Motor::getMode() {
    return mode;
}

boolean Motor::getNeedAttach() const {
    return needAttach;
}

void Motor::setNeedAttach(boolean needAttach) {
    Motor::needAttach = needAttach;
}

uint8_t Motor::getHomePin() const {
    return homePin;
}

