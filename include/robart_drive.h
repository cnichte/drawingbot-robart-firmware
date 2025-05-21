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
  public:
    enum Mode { DRIVING_MODE, PLOT_MODE }; // Mode enumeration

private:
  AccelStepper motor1;
  AccelStepper motor2;
  AccelStepper motor3;
  
  Mode currentMode = PLOT_MODE; // Default to plot mode

  float sqrt3 = 1.73205;
  float r = 0.3; // Distance from robot center to wheel (meters)

  // driving-mode
  float vx = 0.0, vy = 0.0, omega = 0.0; // Velocities for driving mode

  // plot-mode
  float steps_per_meter; // Steps per meter of wheel travel
  float lastX = 0.0, lastY = 0.0; // Track last position

public:

  RobArt_Drive(uint8_t motorInterfaceType, uint8_t stepPin1, uint8_t dirPin1, uint8_t stepPin2, uint8_t dirPin2, uint8_t stepPin3, uint8_t dirPin3);
  void setup(float max_speed, float steps_per_rev, float wheel_diameter);
  void setMode(Mode mode); // Set driving or plot mode
  Mode getMode(); // Return current mode

   // driving-mode 
  void update(float vx, float vy, float  omega, float speed_fak);
  void calculateWheelSpeeds(float *v1, float *v2, float *v3);
  
  // plot-mode
  void moveTo(float x, float y); 
  void calculateWheelPositions(float x, float y, long *steps1, long *steps2, long *steps3);
  bool isMoving(); // Check if motors are still moving
  
  void run(); // Run motors to reach target
};

#endif