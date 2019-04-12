//
// Created by fulva on 2019/3/29.
//

#include <HID.h>
#include "Motor.h"

Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis) : pin1(pin1),
                                                                                                             pin2(pin2),
                                                                                                             pwm_e(pwmE),
                                                                                                             megaEncoderCounter(
                                                                                                                     megaEncoderCounter1),
                                                                                                             axis(axis) {}

void Motor::drive(long speed) {
    if (speed > 0) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        if (speed > 255) {
            analogWrite(pwm_e, 255);
        } else {
            analogWrite(pwm_e, speed);
        }
    } else {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        if (abs(speed) > 255) {
            analogWrite(pwm_e, 255);
        } else {
            analogWrite(pwm_e, abs(speed));
        }

    }
}

void Motor::goTo(long destination) {
    this->destinations[0] = destination;
}

void Motor::activate() {
    int position;
    switch (axis) {
        case 'x':
            position = megaEncoderCounter.XAxisGetCount();
            break;
        case 'y':
            position = megaEncoderCounter.YAxisGetCount();
            break;
        default:
//            should not be here
            break;
    }
    if (mode == OSCILLATE && abs(destinations[0] - position) <= 50) {
        int swap;
        swap = destinations[0];
        destinations[0] = destinations[1];
        destinations[1] = swap;
    }
    if (abs(destinations[0] - position) > 50) {
        drive(destinations[0] - position);
        Serial.print("run left:");
        Serial.println(abs(destinations[0] - position));
    } else {
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
            return megaEncoderCounter.XAxisGetCount();
        case 'y':
            return megaEncoderCounter.YAxisGetCount();
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
