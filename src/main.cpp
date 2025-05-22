/******************************************************
 *
 * Project: RobArt the DrawingBot
 * The main program
 *
 * Copyright (C) 2025 [Carsten Nichte]
 * https://carsten-nichte.de/docs/drawingbot/
 * This file is part of Robart.
 * Licensed under the GNU General Public License v3.0. See LICENSE for details.
 *
 ******************************************************/

/******************************************************
 * Hardware:
 ******************************************************
 Arduino Pro mini Atmega328 (5V, 16mHz)
 + USB to TTL FTDI FT232RL Adapter (Programmiergerät)
 - HM-10, Bluetooth 4.0, BLE-compatible
   - SparkFun Logic Level Konverter, bidirektional
 - DRV8825 Stepper Motor driver
   - Hybridschrittmotor NEMA 17 , 1,8° , 2,5A , 3,1V
 - PCA9685 - 16 channel 12 bit PWM servo driver
   - 2x5 SG92R Servos
 - Kompass GY-91

******************************************************
* G-Code Examples:
******************************************************

Example for PENs + plot:
------------------------------------
T2         ; Select pen 2
M5         ; Pen up
G1 X10 Y10 ; Move to (10,10)
M3         ; Pen down
M7 S45     ; Set angle to 45°
G1 X20 Y20 ; Draw to (20,20)
M5         ; Pen up

Example for Mode switching:
------------------------------------
M100 S0    ; Switch to driving mode
G1 X50 Y20 ; Drive with velocity
M100 S1    ; Switch to plot mode
G1 X10 Y10 ; Move to position

******************************************************
* TODOs
******************************************************

- Motoren auschalten wenn alle drei stehen.
- Richtung umschalten?
- Microstepping per Software ändern (Kommt erst mit Maximilians Treiber)
- ? HardwareSerial für Bluetooth verwenden (und für den Serial Monitor SoftwareSerial verwenden)
- Etwas zurück geben (hat schon mal funktioniert)
Erledigt:
- Ich muss Schritte zählen:plot-mode. Done!

*******************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "robart_drive.h"
#include "robart_bluetooth_serial.h"
#include "robart_bluetooth_parser.h"
#include "robart_pen_manager.h"
#include "ChangeReporter.h"

// Define stepper motor connections and motor interface type.
// Motor interface type must be set to 1 when using a driver.
#define motorInterfaceType 1

// Motor 1 - Pins for direction and steps
#define dirPin1 2
#define stepPin1 3
// Motor 2 - Pins for direction and steps
#define dirPin2 4
#define stepPin2 5
// Motor 3 - Pins for direction and steps
#define dirPin3 6
#define stepPin3 7

#define motorEnable 8 // Pin to enable|disable all 3 motor drivers
#define isFree_9 9    // unused pin

#define btRxPin 10 // RX pin for Bluetooth module
#define btTxPin 11 // TX pin for Bluetooth module

#define isFree_12 12 // unused pin
#define idFree_13 13 // unused pin

ChangeReporter statusReporter;
// Bluetooth Modul an RX=2, TX=3
RobArt_BluetoothSerial bluetooth(btRxPin, btTxPin);
// The Command Parser
RobArt_Parser *parser = nullptr;
// RobArt_drive Instanz to manage the three stepper motors
RobArt_Drive drive = RobArt_Drive(motorInterfaceType, stepPin1, dirPin1, stepPin2, dirPin2, stepPin3, dirPin3);
// Pen control
RobArt_PenManager penManager;

// Arduino Setup Function
// The setup function is where you initialize your variables, pin modes, start using libraries, etc.
// It will only run once, after each power-up or reset of the Arduino board
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // Wait for connection (necessary for some boards)
  Serial.println("Hello Serial Monitor!");

  bluetooth.begin(); // Defaults to 9600 baud
  Serial.println("Bluetooth initialized.");

  parser = new RobArt_Parser(bluetooth.getStream());
  Serial.println("Parser initialized.");

  drive.setup(1000, 3200, 0.05); // 1000 max 4000, 3200 steps/rev, 50mm wheel diameter
  Serial.println("Drive initialized.");

  penManager.begin(); // Initialize PCA9685 PWM-Servo-Driver
  // Add 5 pens (10 servos, channels 0–9)
  // liftChannel, angleChannel, liftMin, liftMax, angleMin, angleMax
  penManager.addPen(0, 0, 1, 500, 1500, 1000, 2000); // Pen 0: lift ch0, angle ch1
  penManager.addPen(1, 2, 3, 500, 1500, 1000, 2000); // Pen 1: lift ch2, angle ch3
  penManager.addPen(2, 4, 5, 500, 1500, 1000, 2000); // Pen 2: lift ch4, angle ch5
  penManager.addPen(3, 6, 7, 500, 1500, 1000, 2000); // Pen 3: lift ch6, angle ch7
  penManager.addPen(4, 8, 9, 500, 1500, 1000, 2000); // Pen 4: lift ch8, angle ch9
  Serial.println("PenManager initialized.");
}

// The loop function is where the program runs continuously
void loop()
{
  if (parser)
  {
    parser->update(); // read incoming data from Bluetooth and parse the stuff

    parser->onMove([](int x, int y)
    {
        if (drive.getMode() == RobArt_Drive::PLOT_MODE) {
            Serial.println("Plot Mode - Moving to X: " + String(x, 1) + " Y: " + String(y, 1));
            drive.moveTo(x, y);
        } else {
            Serial.println("Driving Mode - Velocity X: " + String(x, 1) + " Y: " + String(y, 1));
            drive.update(x, y, 0, 10); // Use x, y as velocities
        }
    });

    parser->onPenControl([](bool lift, float value) {
      if (!lift) {
        penManager.setLift(false); // M5: Pen up
        Serial.println("Pen up");
      } else if (value >= 0 && value <= 180) {
        penManager.setAngle(value); // M7 S<angle>: Set angle
        Serial.println("Set angle to: " + String(value, 1));
      } else {
        penManager.setLift(true); // M3: Pen down
        Serial.println("Pen down");
      }
    });

    parser->onPenSelect([](uint8_t penId) {
        penManager.selectPen(penId);
        Serial.println("Selected Pen: " + String(penId));
    });

    parser->onMode([](uint8_t mode) {
        if (mode == 0) {
            drive.setMode(RobArt_Drive::DRIVING_MODE);
            Serial.println("Switched to Driving Mode");
        } else if (mode == 1) {
            drive.setMode(RobArt_Drive::PLOT_MODE);
            Serial.println("Switched to Plot Mode");
        }
    });

  }
  
  drive.run(); // Run the motors
}

/* 
------------------------------------
main.cpp Minimal Example (that works)
------------------------------------

#include <AccelStepper.h>
#define motorInterfaceType 1
AccelStepper motor1(motorInterfaceType, 3, 2);
AccelStepper motor2(motorInterfaceType, 5, 4);
AccelStepper motor3(motorInterfaceType, 7, 6);

void setup() {
    Serial.begin(9600);
    motor1.setMaxSpeed(1000);
    motor2.setMaxSpeed(1000);
    motor3.setMaxSpeed(1000);
    motor1.setSpeed(250);
    motor2.setSpeed(500);
    motor3.setSpeed(1000);
}

void loop() {
    motor1.runSpeed();
    motor2.runSpeed();
    motor3.runSpeed();
}

*/