//
// Created by fulva on 5/10/19.
//

#include "PIController.h"

int PIController::target(int error) {
    accuError += error;
    return Kp * error + Ki * accuError;
}

PIController::PIController(double kp, double ki) : Kp(kp), Ki(ki) {}
