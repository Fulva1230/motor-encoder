//
// Created by fulva on 5/10/19.
//

#include <HID.h>
#include "PIController.h"

int PIController::target(int error) {
    //TODO CAN BE TOO LARGE TIMEINTERVALE
    unsigned int timeInterval = millis() - lastTime;
    accuError += error * (timeInterval);
    inteTime += timeInterval;
    return Kp * error + Ki * accuError / inteTime;
}

PIController::PIController(double kp, double ki) : Kp(kp), Ki(ki) {}
