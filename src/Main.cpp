//
// Created by fulva on 2019/3/29.
//

#include <Arduino.h>
#include <MegaEncoderCounter.h>
#include "Motor.h"

#define PIN_1 1
#define PIN_2 5
#define PINA_PWM 53
#define PIN_3 10
#define PIN_4 11
#define PINB_PWM 7
MegaEncoderCounter megaEncoderCounter;
Motor motor1(PIN_1, PIN_2, PINA_PWM, megaEncoderCounter, 'x', 0);
Motor motor2(PIN_3, PIN_4, PINB_PWM, megaEncoderCounter, 'y', 0);

void setup() {
    Serial.begin(9600);
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PINA_PWM, OUTPUT);
    megaEncoderCounter.XAxisReset();
    megaEncoderCounter.YAxisReset();
}

void interrupt1() {
    motor1.setMode(Motor::STOP);
}

void interrupt2() {
    motor2.setMode(Motor::STOP);
}

void loop() {
    String command;
    char h;
    int number;
    short motor_sel;
    if (Serial.available()) {
        command = Serial.readString();
        h = command[0];
        motor_sel = command.substring(1, 2).toInt();
        number = command.substring(2).toInt();
        Motor *motor;
        switch (motor_sel) {
            case 1:
                motor = &motor1;
                break;
            case 2:
                motor = &motor2;
                break;
            default:
                motor = &motor1;
        }
        switch (h) {
            case 'R':
                motor->resetAxis();
                Serial.println("Reset Axis");
                break;
            case 'G':
                motor->setMode(Motor::GO);
                Serial.println("go to");
                motor->goTo(number);
                break;
            case 'O':
                motor->setMode(Motor::OSCILLATE);
                Serial.println("set oscillate dis1:");
                Serial.println(number);
                motor->setFirstDest(number);
                break;
            case 'S':
                motor->setMode(Motor::OSCILLATE);
                Serial.println("set oscillate dis2:");
                Serial.println(number);
                motor->setSecondDest(number);
                break;
            case 'P':
                Serial.print("current position: ");
                Serial.println(motor->getAxis());
                break;
            case 'T':
                motor->setMode(Motor::STOP);
                break;
            case 'H':
                motor->setMode(Motor::HOME);
                detachInterrupt(digitalPinToInterrupt(motor->getHomePin()));
                motor->setNeedAttach(false);
        }
    }
    if (motor1.getNeedAttach()) {
        attachInterrupt(digitalPinToInterrupt(motor1.getHomePin()), interrupt1, RISING);
        motor1.setNeedAttach(false);
    }
    if (motor2.getNeedAttach()) {
        attachInterrupt(digitalPinToInterrupt(motor2.getHomePin()), interrupt2, RISING);
        motor2.setNeedAttach(false);
    }
    motor1.activate();
    motor2.activate();
}
