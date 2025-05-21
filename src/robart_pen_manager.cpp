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

/**
 * Initialize the PCA9685 PWM driver.
 * Parameters:
 * - i2cAddress: I2C address of the PCA9685 (default is 0x40)
 * 
 * The i2cAddress parameter defaults to 0x40, 
 * which is the standard I2C address for the PCA9685 when its address pins (A0–A5) 
 * are all set to LOW (default configuration).
 * 
 * If your PCA9685 module has a different address (e.g., due to jumper settings on A0–A5), 
 * you can pass a different address when calling this function.
 * 
 * The PCA9685 supports addresses from 0x40 to 0x7F, 
 * depending on the configuration of the address pins. 
 * Check your module’s documentation or schematic 
 * to confirm the address (e.g., soldering jumpers A0–A5).
 */
void RobArt_PenManager::begin(uint8_t i2cAddress = 0x40) {
    pwm = Adafruit_PWMServoDriver(i2cAddress);
    pwm.begin();
    pwm.setPWMFreq(50); // Standard 50Hz for servos
}

/**
 * Add a pen to the manager.
 * Parameters:
 * - penId: ID of the pen (0-4)
 * - liftChannel: PWM channel for lifting the pen
 * - angleChannel: PWM channel for adjusting the angle
 * - liftMin: Minimum lift position
 * - liftMax: Maximum lift position
 * - angleMin: Minimum angle position
 * - angleMax: Maximum angle position
 */
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