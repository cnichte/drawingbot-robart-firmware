/******************************************************
 * 
 * Project: RobArt the DrawingBot
 *  
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 * 
 ******************************************************/

#ifndef ROBART_PEN_MANAGER_H
#define ROBART_PEN_MANAGER_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "robart_pen.h"

#define MAX_PENS 5

class RobArt_PenManager {
public:
    RobArt_PenManager();
    void begin(uint8_t i2cAddress = 0x40); // Default PCA9685 address
    void addPen(uint8_t penId, uint8_t liftChannel, uint8_t angleChannel,
                uint16_t liftMin, uint16_t liftMax, uint16_t angleMin, uint16_t angleMax);
    void selectPen(uint8_t penId); // Select active pen (0–4)
    void setLift(bool down); // Control active pen’s lift
    void setAngle(float angle); // Control active pen’s angle

private:
    Adafruit_PWMServoDriver pwm;
    RobArt_Pen* pens[MAX_PENS];
    uint8_t activePenId;
};

#endif