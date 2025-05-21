/******************************************************
 * 
 * Project: RobArt the DrawingBot
 *  
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 * 
 ******************************************************/

#ifndef ROBART_PEN_H
#define ROBART_PEN_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

class RobArt_Pen {
public:
    RobArt_Pen(Adafruit_PWMServoDriver &pwm, uint8_t liftChannel, uint8_t angleChannel,
               uint16_t liftMin, uint16_t liftMax, uint16_t angleMin, uint16_t angleMax);
    void setLift(bool down); // true = down, false = up
    void setAngle(float angle); // Angle in degrees (0–180)

private:
    Adafruit_PWMServoDriver &pwm;
    uint8_t liftChannel, angleChannel;
    uint16_t liftMin, liftMax; // PWM pulse widths (e.g., 500–1500µs)
    uint16_t angleMin, angleMax; // PWM pulse widths (e.g., 1000–2000µs)
    uint16_t mapAngleToPWM(float angle); // Map 0–180° to PWM
};

#endif