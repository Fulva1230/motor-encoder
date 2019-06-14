//
// Created by fulva on 5/25/19.
//

#include "Curve.h"

Curve::Curve(long initPos, long finalPos) : initPos(initPos), finalPos(finalPos) {}

long Curve::getNowDest(long time) {
    return getNowDest(time, initPos, finalPos);
}
