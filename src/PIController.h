//
// Created by fulva on 5/10/19.
//

#ifndef MOTOR_ENCODER_PICONTROLLER_H
#define MOTOR_ENCODER_PICONTROLLER_H


#include <stdint-gcc.h>

class PIController {
public:
    PIController(double kp, double ki);

    int target(int error);

private:
    double Kp;
    double Ki;
    unsigned long inteTime{0};
    unsigned long lastTime{0};
    double accuError{0};

};


#endif //MOTOR_ENCODER_PICONTROLLER_H
