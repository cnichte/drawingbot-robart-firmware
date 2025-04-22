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
RobArt_Bluetooth_Parser parser(bluetooth.getStream());

// RobArt_drive Instanz to manage the three stepper motors
RobArt_Drive drive = RobArt_Drive(motorInterfaceType, stepPin1, dirPin1, stepPin2, dirPin2, stepPin3, dirPin3);

// Stift-Ansteuerung
char pen_nr;
char pen_command;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // Warte auf Verbindung (bei manchen Boards nötig)
  Serial.println("Hallo Serial Monitor!");

  bluetooth.begin(); // Defaults to 9600 baud
  Serial.println("Bluetooth initialized");

  parser.onMove([](int x, int y) {
    // Deine Bewegungslogik
    Serial.print("Move to X: "); Serial.print(x);
    Serial.print(" Y: "); Serial.println(y);
  });

  parser.onLed([](bool on, int pwm) {
    analogWrite(3, on ? pwm : 0);
    digitalWrite(13, on ? HIGH : LOW);
  });

  parser.onStatus([]() {
    bluetooth.println("OK T:23.7");
  });

  drive.setup(1000); // max 4000
}


void loop()
{
  // put your main code here, to run repeatedly:

  String status = bluetooth.available() ? "ja" : "nein";
  statusReporter.reportIfChanged(status);

  parser.update();
  drive.update(0); // muss dann oben in die Handler rein.

  // TODO Das wird komplett durch den parser ersetzt
  if (bluetooth.available())
  {
        Serial.println("Bluetooth verfügbar");
        // String msg = Serial.readStringUntil('\n');
        String msg = bluetooth.receive(); // ! Das ging!
        Serial.print("Empfangene Daten: ");
        Serial.println(msg);
        bluetooth.send("Echo: " + msg);

        // das da ersetzt die lo
        
  }

  // delay(10);
}

    /*
        if (msg.startsWith("PATH"))
        {
          Parser.parseSVGPath(msg.substring(5), &drive.vx, &drive.vy, &drive.lastX, &drive.lastY);
        }
        else if (msg.startsWith("PEN"))
        {
          // PEN 1|2|3|4|5|ALL UP|DOWN|TOGGLE
          Parser.parsePEN(msg.substring(4));
        }
        else
        {
          sscanf(msg.c_str(), "%f %f %f", &drive.vx, &drive.vy, &drive.omega);
        }
    */