//
// Created by fulva on 2019/3/29.
//

#include <Arduino.h>
#include <MegaEncoderCounter.h>
#include "Motor.h"

#define PIN_1 5
#define PIN_2 6
#define PINA_PWM 7
#define PIN_3 8
#define PIN_4 9
#define PINB_PWM 10
MegaEncoderCounter megaEncoderCounter;
Motor motor1(PIN_1, PIN_2, PINA_PWM, megaEncoderCounter, 'x');
Motor motor2(PIN3, PIN4, PINB_PWM, megaEncoderCounter, 'y');

void setup() {
    Serial.begin(9600);
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PINA_PWM, OUTPUT);
    megaEncoderCounter.XAxisReset();
    megaEncoderCounter.YAxisReset();
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
        number = command.substring(2).toInt() / 360.0 * 4440;
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
        }
    }
    motor1.activate();
    motor2.activate();
}
