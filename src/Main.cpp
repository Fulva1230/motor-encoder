//
// Created by fulva on 2019/3/29.
//

#include <Arduino.h>
#include <MegaEncoderCounter.h>
#include "Motor.h"
#include "aCurve.h"

#define a2c1 15.17 // angle to count, unit:count/angle 5460/360
#define a2c2 29.5555555 // angle to count, unit:count/angle 10640/360, angle * (count/angle) = count
#define PIN_1 11
#define PIN_2 10
#define PINA_PWM 7
#define PIN_3 9
#define PIN_4 8
#define PINB_PWM 6
#define a1 15
#define a2 5
MegaEncoderCounter megaEncoderCounter;
Motor motor2(PIN_1, PIN_2, PINA_PWM, megaEncoderCounter, 'y', 3, a2c2, 90, 180);
Motor motor1(PIN_3, PIN_4, PINB_PWM, megaEncoderCounter, 'x', 2, a2c1, 130, 90);
struct Angles {
    float th1;
    float th2;
};

void interrupt1() {
    if (!motor1.getNeedAttach()) {
        motor1.setMode(Motor::STOP);
    }
}

void interrupt2() {
    if (!motor2.getNeedAttach()) {
        motor2.setMode(Motor::STOP);
    }

}

bool angleVerify(float th1, float th2) {
    return th1 >= -90 && th1 <= 90;
}

///
/// \param x
/// \param y
/// \param cth1 the current angle of the motor 1
/// \param cth2 the current angle of the motor 2
/// \return the destination of motors
Angles transpose(float x, float y, float cth1, float cth2) {
    float th2 = acos((x * x + y * y - a1 * a1 - a2 * a2) / (2 * a1 * a2));
    float th1 = atan2(y, x) - atan2(a2 * sin(th2), a1 + a2 * cos(th2));
    float th22 = -acos((x * x + y * y - a1 * a1 - a2 * a2) / (2 * a1 * a2));
    float th12 = atan2(y, x) - atan2(a2 * sin(th22), a1 + a2 * cos(th22));
//    x = a1 * cos(th1) + a2 * cos(th1 + th2);
//    Serial.print("x :");
//    Serial.println(x);
//    y = a1 * sin(th1) + a2 * sin(th1 + th2);
//    Serial.print("y :");
//    Serial.println(y);
//    int x2 = a1 * cos(th12) + a2 * cos(th12 + th22);
//    int y2 = a1 * sin(th12) + a2 * sin(th12 + th22);
    th1 = (th1 * 180) / 3.14;
    th2 = (th2 * 180) / 3.14;
    th12 = (th12 * 180) / 3.14;
    th22 = (th22 * 180) / 3.14;
    Serial.println(th1);
    Serial.println(th2);
    Serial.println(th12);
    Serial.println(th22);
//    because the rotating of the bigger motor consume more energy than the smaller one, so if there is two choices of angles, we choose the angle is small for motor 1
    if (angleVerify(th1, th2) && angleVerify(th12, th22)) {
        if (abs(th1 - cth1) < abs(th12 - cth1)) {
            return Angles{.th1=th1, .th2=th2};
        } else {
            return Angles{.th1=th12, .th2=th22};
        }
    } else if (angleVerify(th1, th2)) {
        return Angles{.th1=th1, .th2=th2};
    } else if (angleVerify(th12, th22)) {
        return Angles{.th1=th12, .th2=th22};
    }
    return Angles{.th1=0, .th2=0};

};

void waitUntilAva() {
    while (!Serial.available()) {
        delay(50);
    }
}


void setup() {
    Serial.begin(9600);
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PINA_PWM, OUTPUT);
    megaEncoderCounter.XAxisReset();
    attachInterrupt(digitalPinToInterrupt(motor1.getHomePin()), interrupt1, RISING);
    megaEncoderCounter.YAxisReset();
    attachInterrupt(digitalPinToInterrupt(motor2.getHomePin()), interrupt2, RISING);
    {
        Serial.println("set first motor kp : ");
        waitUntilAva();
        double kp = Serial.readStringUntil('\n').toDouble();
        Serial.println(kp);
        Serial.println("set first motor ki : ");
        waitUntilAva();
        double ki = Serial.readStringUntil('\n').toDouble();
        Serial.println(ki);
        motor1.setPI(PIController(kp, ki));
        Serial.println("set second motor kp :");
        waitUntilAva();
        kp = Serial.readStringUntil('\n').toDouble();
        Serial.println(kp);
        Serial.println("set second motor ki : ");
        waitUntilAva();
        ki = Serial.readStringUntil('\n').toDouble();
        Serial.println(ki);
        motor2.setPI(PIController(kp, ki));
    }
}

void loop() {
//    Serial.print("pin2 is");
//    Serial.println(digitalRead(3));
    String command;
    char h;
    int numbers[5];
    int index(0);
    short motor_sel;
    if (Serial.available()) {
        command = Serial.readString();
        h = command[0];
        motor_sel = command.substring(1, 2).toInt();
        String left = command.substring(2);
        while (left.indexOf(':') != -1) {
            numbers[index] = left.substring(0, left.indexOf(':')).toInt();
            left = left.substring(left.indexOf(':') + 1);
            index++;
        }
        numbers[index] = left.toInt();
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
            case 'E':
                motor->alongTcurve(new aCurve(motor->getAxisAngle(), numbers[0], 10, numbers[1]));
                Serial.println("drive T-curve");
                break;
            case 'A':
                motor->driveAngle(numbers[0]);
                Serial.print("drive angle: ");
                Serial.println(numbers[0]);
                break;
            case 'R':
                motor->resetAxis();
                Serial.println("Reset Axis");
                break;
            case 'G':
                Serial.println("go to");
                motor->goTo(numbers[0]);
                break;
            case 'O':
                Serial.println("set oscillate dis1:");
                Serial.println(numbers[0]);
                motor->setFirstDest(numbers[0]);
                break;
            case 'S':
                Serial.println("set oscillate dis2:");
                Serial.println(numbers[0]);
                motor->setSecondDest(numbers[0]);
                break;
            case 'P':
                Serial.print("current position: ");
                Serial.println(motor->getAxisAngle());
                break;
            case 'T':
                motor->setMode(Motor::STOP);
                break;
            case 'H':
                Serial.println("go home");
                motor->setMode(Motor::HOME);
                detachInterrupt(digitalPinToInterrupt(motor->getHomePin()));
                motor->setNeedAttach(false);
                break;
            case 'I':
                Angles angles = transpose(numbers[0], numbers[1], motor1.getAxisAngle(), motor2.getAxisAngle());
                motor1.alongTcurve(new aCurve(motor1.getAxisAngle(), angles.th1, 10, 5000));
                motor2.alongTcurve(new aCurve(motor2.getAxisAngle(), angles.th2, 10, 5000));
                Serial.print("Go to position :");
                Serial.print(numbers[0]);
                Serial.print("::");
                Serial.println(numbers[1]);
                Serial.print("result th1 :");
                Serial.print(angles.th1);
                Serial.print("th2 :");
                Serial.println(angles.th2);
                break;
        }
    }
    if (motor1.getNeedAttach()) {
        attachInterrupt(digitalPinToInterrupt(motor1.getHomePin()), interrupt1, CHANGE);
        motor1.setNeedAttach(false);
    }
    if (motor2.getNeedAttach()) {
        attachInterrupt(digitalPinToInterrupt(motor2.getHomePin()), interrupt2, CHANGE);
        motor2.setNeedAttach(false);
    }
    motor1.activate();
    motor2.activate();
}
