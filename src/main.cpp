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

// Hardware:
// Arduino Pro mini Atmega328 (5V, 16mHz)
// + USB to TTL FTDI FT232RL Adapter (Programmiergerät)
// - HM-10, Bluetooth 4.0, BLE-compatible
//   - SparkFun Logic Level Konverter, bidirektional
// - DRV8825 Stepper Motor driver
//   - Hybridschrittmotor NEMA 17 , 1,8° , 2,5A , 3,1V
// - PCA9685 - 16 channel 12 bit PWM servo driver
//   - SG92R Servos
// - Kompass GY-91

#include "robart_drive.h"
#include "robart_bluetooth_serial.h"
#include "robart_bluetooth_parser.h"
#include "ChangeReporter.h"

// Define stepper motor connections and motor interface type.
// Motor interface type must be set to 1 when using a driver:
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

ChangeReporter statusReporter;
// Bluetooth Modul an RX=2, TX=3
RobArt_BluetoothSerial bluetooth(10, 11);
// The Command Parser
RobArt_Parser *parser = nullptr;
// RobArt_drive Instanz to manage the three stepper motors
RobArt_Drive drive = RobArt_Drive(motorInterfaceType, stepPin1, dirPin1, stepPin2, dirPin2, stepPin3, dirPin3);
// Pen control
char pen_nr;
char pen_command;
int counter = 0;

// Arduino Setup Function
// The setup function is where you initialize your variables, pin modes, start using libraries, etc.
// The setup function will only run once, after each power-up or reset of the Arduino board
// It initializes the serial communication, the Bluetooth module, and the parser
// and sets up the drive with a maximum speed
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // Wait for connection (necessary for some boards)
  Serial.println("Hello Serial Monitor!");

  bluetooth.begin(); // Defaults to 9600 baud
  Serial.println("Bluetooth initialized.");

  Serial.println("Parser initialized.");
  parser = new RobArt_Parser(bluetooth.getStream());

  Serial.println("Drive initialized.");
  drive.setup(1000); // 1000 max 4000
}

// The loop function is where the program runs continuously
// The loop function also handles the pen control and status reporting
// It uses the parser to handle commands and callbacks for movement, LED control, and status
// The parser is updated in each loop iteration
// The parser checks for incoming commands and calls the appropriate callback functions
void loop()
{
  if (parser)
  {
    parser->update();

    parser->onMove([](int x, int y)
    {
      // Movement logic
      Serial.println("Moving to X: " + String(x) + " Y: " + String(y));
      drive.update(x, y, 0, 10); 
    });

    parser->onLed([](bool on, int pwm)
    {
      // analogWrite(3, on ? pwm : 0);
      // digitalWrite(13, on ? HIGH : LOW);
    });

    parser->onStatus([]()
    { 
      bluetooth.println("OK T:23.7");
    });
  }
  //! delay(10);
}