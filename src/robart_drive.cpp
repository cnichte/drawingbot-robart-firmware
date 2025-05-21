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

#include "robart_drive.h"

/**
 * @brief Constructor for the RobArt_Drive class.
 */
RobArt_Drive::RobArt_Drive(uint8_t motorInterfaceType,
                           uint8_t stepPin1, uint8_t dirPin1,
                           uint8_t stepPin2, uint8_t dirPin2,
                           uint8_t stepPin3, uint8_t dirPin3)
{
  motor1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
  motor2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);
  motor3 = AccelStepper(motorInterfaceType, stepPin3, dirPin3);
}

/**
 * Set the maximum speed in steps per second:
 * depends on your processor and clock speed.
 * max 4000 may be ok
 *
 * @brief Setup the drive with the maximum speed.
 */
void RobArt_Drive::setup(float max_speed, float steps_per_rev, float wheel_diameter)
{
  motor1.setMaxSpeed(max_speed);
  motor2.setMaxSpeed(max_speed);
  motor3.setMaxSpeed(max_speed);

  motor1.setAcceleration(1000); // Adjust as needed
  motor2.setAcceleration(1000);
  motor3.setAcceleration(1000);
  steps_per_meter = steps_per_rev / (wheel_diameter * PI); // Steps per meter
}

void RobArt_Drive::setMode(Mode mode) {
    currentMode = mode;
    if (mode == DRIVING_MODE) {
        motor1.setSpeed(0); // Reset speeds for driving mode
        motor2.setSpeed(0);
        motor3.setSpeed(0);
    }
}

RobArt_Drive::Mode RobArt_Drive::getMode() {
    return currentMode;
}

void RobArt_Drive::moveTo(float x, float y) {
    if (currentMode != PLOT_MODE) return; // Only valid in plot mode
    long steps1, steps2, steps3;
    calculateWheelPositions(x, y, &steps1, &steps2, &steps3);
    motor1.moveTo(steps1);
    motor2.moveTo(steps2);
    motor3.moveTo(steps3);
    lastX = x;
    lastY = y;
    Serial.println("Target steps - Motor1: " + String(steps1) + ", Motor2: " + String(steps2) + ", Motor3: " + String(steps3));
}

/**
 * Update the drive with the current speed.
 * The speed is calculated from the velocity and the radius of the wheel.
 *
 * @brief Update the drive with the current speed.
 */
void RobArt_Drive::update(float vx, float vy, float  omega, float speed_fak)
{
  if (currentMode != DRIVING_MODE) return; // Only valid in driving mode

  this->vx = vx;
  this->vy = vy;
  this->omega = omega;

  this->vx = vx;
  this->vy = vy;
  this->omega = omega;

  // Calculate the wheel speeds
  float v1, v2, v3; // store the calculation results

  calculateWheelSpeeds(&v1, &v2, &v3);
  
  Serial.println("Calculated v1: " + String(v1) + ", v2: " + String(v2) + ", v3: " + String(v3));
  Serial.println("Scaled     v1: " + String(v1 * speed_fak) + ", v2: " + String(v2 * speed_fak) + ", v3: " + String(v3 * speed_fak));
  Serial.println("----------------------------------");

  // Transferring wheel speeds to the motors
  motor1.setSpeed(v1 * speed_fak);
  motor2.setSpeed(v2 * speed_fak);
  motor3.setSpeed(v3 * speed_fak);
}

/**
 * Run the motors with the current speed.
 * The motors are run with the current speed.
 * This must be called in the loop() function.
 * 
 * @brief Run the motors with the current speed.
 */
void RobArt_Drive::run()
{
    if (currentMode == PLOT_MODE) {
        motor1.run();
        motor2.run();
        motor3.run();
    } else {
        motor1.runSpeed();
        motor2.runSpeed();
        motor3.runSpeed();
    }
}

bool RobArt_Drive::isMoving() {
    if (currentMode == PLOT_MODE) {
        return motor1.isRunning() || motor2.isRunning() || motor3.isRunning();
    }
    return true; // In driving mode, assume continuous motion
}

void RobArt_Drive::calculateWheelPositions(float x, float y, long *steps1, long *steps2, long *steps3) {
    float dx = x - lastX;
    float dy = y - lastY;
    float omega = 0; // No rotation for simplicity
    float v1 = dx + r * omega;
    float v2 = -0.5 * dx + (sqrt3 / 2) * dy + r * omega;
    float v3 = -0.5 * dx - (sqrt3 / 2) * dy + r * omega;
    *steps1 = (long)(v1 * steps_per_meter) + motor1.currentPosition();
    *steps2 = (long)(v2 * steps_per_meter) + motor2.currentPosition();
    *steps3 = (long)(v3 * steps_per_meter) + motor3.currentPosition();
}

/**
 * Calculate the wheel speeds from the velocity and the radius of the wheel.
 * The wheel speeds are calculated from the velocity and the radius of the wheel.
 *
 * @brief Calculate the wheel speeds from the velocity and the radius of the wheel.
 */
void RobArt_Drive::calculateWheelSpeeds(float *v1, float *v2, float *v3)
{
  *v1 = vx + r * omega;
  *v2 = -0.5 * vx + (sqrt3 / 2) * vy + r * omega;
  *v3 = -0.5 * vx - (sqrt3 / 2) * vy + r * omega;
}