//
// Created by fulva on 6/14/19.
//

#include <HID.h>
#include "aCurve.h"

aCurve::aCurve(long initPos, long finalPos, uint16_t ta, uint16_t totalTime) : Curve(initPos, finalPos), ta(ta),
                                                                               totalTime(totalTime) {
    Serial.print("total time is");
    Serial.println(totalTime);
    Serial.print("final pos is");
    Serial.println(finalPos);
}

long aCurve::getNowDest(long time, long initPos, long finalPos) {
    double vmax = (double) (finalPos - initPos) / (totalTime - ta);
    if (time < ta) {
        return initPos + vmax * time * time / (2 * ta);
    } else if (time < totalTime - ta) {
        return initPos + vmax * ta / 2 + (time - ta) * vmax;
    } else if (time < totalTime) {
        return initPos + vmax * ta / 2 + vmax * (totalTime - ta) - vmax * (totalTime - time) / (2 * ta);
    }
    return finalPos;
}
