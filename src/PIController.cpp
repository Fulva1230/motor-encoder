//
// Created by fulva on 5/10/19.
//

#include <HID.h>
#include "PIController.h"

int PIController::target(int error) {
    unsigned int timeInterval = millis() - lastTime;
    if (timeInterval < 200) {
        accuError += error * (timeInterval);
        inteTime += timeInterval;
    }
    double P = Kp * error;
    return P + Ki * accuError / (inteTime ? inteTime : INT32_MAX);
}

PIController::PIController(double kp, double ki) : Kp(kp), Ki(ki) {}
