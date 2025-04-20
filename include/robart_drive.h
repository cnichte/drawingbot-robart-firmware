/******************************************************
 *
 * Project: RobArt the DrawingBot
 *
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 *
 ******************************************************/

#ifndef ROBART_DRIVE_H
#define ROBART_DRIVE_H

#include <Arduino.h>
#include <AccelStepper.h>

// Prototyes
class RobArt_Drive
{
private:
  AccelStepper motor1;
  AccelStepper motor2;
  AccelStepper motor3;

public:
  float sqrt3 = 1.73205;
  float vx = 0.0, vy = 0.0, omega = 0.0, r = 0.3;
  float lastX = 0.0, lastY = 0.0;

  RobArt_Drive(uint8_t motorInterfaceType, uint8_t stepPin1, uint8_t dirPin1, uint8_t stepPin2, uint8_t dirPin2, uint8_t stepPin3, uint8_t dirPin3);
  void setup(float max_speed);
  void update(float speed_fak);
  void calculateWheelSpeeds(float *v1, float *v2, float *v3);
};

#endif