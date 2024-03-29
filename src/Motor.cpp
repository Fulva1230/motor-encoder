//
// Created by fulva on 2019/3/29.
//

#include <HID.h>
#include "Motor.h"


Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmE, MegaEncoderCounter &megaEncoderCounter1, char axis,
             uint8_t homePin, double angToCou, uint8_t maxSpeed, int homeToLimit) : pin1(pin1),
                                                                                    pin2(pin2),
                                                                                    pwm_e(pwmE),
                                                                                    megaEncoderCounter(
                                                                                            megaEncoderCounter1),
                                                                                    axis(axis),
                                                                                    resetResponse(homePin),
                                                                                    homePin(homePin),
                                                                                    angleToCount(angToCou),
                                                                                    maxSpeed(maxSpeed),
                                                                                    homeToLimitAngle(homeToLimit) {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pwmE, OUTPUT);


}

void Motor::drive(long speed) {
    if (speed > 0) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        if (speed > maxSpeed) {
            analogWrite(pwm_e, maxSpeed);
        } else {
            analogWrite(pwm_e, speed);
        }
    } else {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        if (abs(speed) > maxSpeed) {
            analogWrite(pwm_e, maxSpeed);
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
    int speed;
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
            speed = piController.target(destinations[0] - position);
            drive(speed);
            Serial.print("speed :");
            Serial.println(speed);
            Serial.print("run left:");
            Serial.println(abs(destinations[0] - position));
            break;
        case HOME:
//            Serial.print("driving");
            drive(100);
            if (resetResponse.isActivate()) {
                Serial.println("go home completed");
                drive(0);
                resetAxis();
                setNeedAttach(true);
                while (getAxisAngle() > 10) {
                    drive(-100);
                }
                goTo(0);
            }
            break;
        case STOP:
            Serial.println("motor stop");
            drive(0);
            break;
        case T_CURVE:
            speed = piController.target(curve->getNowDest(curveTime) * angleToCount - position);
            ++curveTime;
            drive(speed);
            break;
    }
}

void Motor::resetAxis() {
    destinations[0] = 0;
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
    int tmp;
    switch (axis) {
        case 'x':
            tmp = megaEncoderCounter.XAxisGetCount() > INT32_MAX ? megaEncoderCounter.XAxisGetCount() - UINT32_MAX - 1
                                                                 : megaEncoderCounter.XAxisGetCount();
            break;
        case 'y':
            tmp = megaEncoderCounter.YAxisGetCount() > INT32_MAX ? megaEncoderCounter.YAxisGetCount() - UINT32_MAX - 1
                                                                 : megaEncoderCounter.YAxisGetCount();
            break;
    }
    return tmp + homeToLimitAngle * angleToCount;
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

void Motor::driveAngle(long desAng) {
    goTo(desAng * angleToCount);
}

int Motor::getAxisAngle() {
    return getAxis() / angleToCount;
}

void Motor::setPI(const PIController &controller) {
    piController = controller;
}

void Motor::alongTcurve(Curve *curve) {
    delete this->curve;
    setMode(T_CURVE);
    curveTime = 0;
    this->curve = curve;
}

