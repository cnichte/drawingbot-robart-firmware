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
void RobArt_Drive::setup(float max_speed)
{
  motor1.setMaxSpeed(max_speed);
  motor2.setMaxSpeed(max_speed);
  motor3.setMaxSpeed(max_speed);
}

/**
 * Update the drive with the current speed.
 * The speed is calculated from the velocity and the radius of the wheel.
 *
 * @brief Update the drive with the current speed.
 */
void RobArt_Drive::update(float speed_fak)
{
  float v1, v2, v3; // store the calculation results

  calculateWheelSpeeds(&v1, &v2, &v3);

  // Transferring wheel speeds to the motors
  motor1.setSpeed(v1 * speed_fak);
  motor2.setSpeed(v2 * speed_fak);
  motor3.setSpeed(v3 * speed_fak);

  motor1.runSpeed();
  motor2.runSpeed();
  motor3.runSpeed();
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