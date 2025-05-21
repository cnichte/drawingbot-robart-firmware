/******************************************************
 *
 * Project: RobArt the DrawingBot
 * Parser for Bluetooth Commands
 *
 * Copyright (C) 2025 [Carsten Nichte]
 * https://carsten-nichte.de/docs/drawingbot/
 * This file is part of Robart.
 * Licensed under the GNU General Public License v3.0. See LICENSE for details.
 * 
 ******************************************************/

#include "robart_pen.h"

RobArt_Pen::RobArt_Pen(Adafruit_PWMServoDriver &pwm, uint8_t liftChannel, uint8_t angleChannel,
                       uint16_t liftMin, uint16_t liftMax, uint16_t angleMin, uint16_t angleMax)
    : pwm(pwm), liftChannel(liftChannel), angleChannel(angleChannel),
      liftMin(liftMin), liftMax(liftMax), angleMin(angleMin), angleMax(angleMax) {}

void RobArt_Pen::setLift(bool down) {
    uint16_t pulse = down ? liftMax : liftMin;
    pwm.setPWM(liftChannel, 0, pulse);
}

void RobArt_Pen::setAngle(float angle) {
    angle = constrain(angle, 0, 180); // Limit to 0–180°
    uint16_t pulse = mapAngleToPWM(angle);
    pwm.setPWM(angleChannel, 0, pulse);
}

uint16_t RobArt_Pen::mapAngleToPWM(float angle) {
    return map(angle, 0, 180, angleMin, angleMax); // Linear mapping
}