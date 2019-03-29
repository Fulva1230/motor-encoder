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
    this->destination = destination;
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
    }
    if (abs(destination - position) > 100) {
        drive(destination - position);
        Serial.print("run left:");
        Serial.println(abs(destination - position));
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
