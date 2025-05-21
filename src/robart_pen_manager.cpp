/******************************************************
 *
 * Project: RobArt the DrawingBot
 * Pen Manager
 *
 * Copyright (C) 2025 [Carsten Nichte]
 * https://carsten-nichte.de/docs/drawingbot/
 * This file is part of Robart.
 * Licensed under the GNU General Public License v3.0. See LICENSE for details.
 * 
 ******************************************************/

#include "robart_pen_manager.h"

RobArt_PenManager::RobArt_PenManager() : activePenId(0) {
    for (int i = 0; i < MAX_PENS; i++) {
        pens[i] = nullptr;
    }
}

void RobArt_PenManager::begin(uint8_t i2cAddress) {
    pwm = Adafruit_PWMServoDriver(i2cAddress);
    pwm.begin();
    pwm.setPWMFreq(50); // Standard 50Hz for servos
}

void RobArt_PenManager::addPen(uint8_t penId, uint8_t liftChannel, uint8_t angleChannel,
                               uint16_t liftMin, uint16_t liftMax, uint16_t angleMin, uint16_t angleMax) {
    if (penId < MAX_PENS) {
        pens[penId] = new RobArt_Pen(pwm, liftChannel, angleChannel, liftMin, liftMax, angleMin, angleMax);
    }
}

void RobArt_PenManager::selectPen(uint8_t penId) {
    if (penId < MAX_PENS && pens[penId] != nullptr) {
        activePenId = penId;
    }
}

void RobArt_PenManager::setLift(bool down) {
    if (pens[activePenId] != nullptr) {
        pens[activePenId]->setLift(down);
    }
}

void RobArt_PenManager::setAngle(float angle) {
    if (pens[activePenId] != nullptr) {
        pens[activePenId]->setAngle(angle);
    }
}