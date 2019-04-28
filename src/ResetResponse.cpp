//
// Created by fulva on 4/26/19.
//

#include <HID.h>
#include "ResetResponse.h"

ResetResponse::ResetResponse(uint8_t homePin) : homePin(homePin) {}

bool ResetResponse::isActivate() {
    return digitalRead(homePin) == HIGH;
}
