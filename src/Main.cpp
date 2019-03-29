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
        }
        switch (h) {
            case 'R':
                motor->resetAxis();
                break;
            case 'G':
                Serial.println("go to");
                motor->goTo(number);
                break;
            case 'P':
                Serial.print("current position: ");
                Serial.println(megaEncoderCounter.XAxisGetCount());
        }
    }
    motor1.activate();
    motor2.activate();
}
