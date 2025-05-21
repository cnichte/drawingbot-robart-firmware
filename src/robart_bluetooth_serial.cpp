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

#include "robart_bluetooth_serial.h"

RobArt_BluetoothSerial::RobArt_BluetoothSerial(uint8_t rxPin, uint8_t txPin) : rx(rxPin), tx(txPin), btStream(nullptr) {
    // after ':' = Initializes the class members rx, tx with the value of the constructor argument. 
    // btStream is initialized by setting it to zero.
}

void RobArt_BluetoothSerial::begin(long baudRate)
{
    btStream = new SoftwareSerial(rx, tx);
    btStream->begin(baudRate);
    
    // TODO: Das funktioniert noch nicht...?
    /*
    btStream->print("AT+ROBARTCounterBLE");
    delay(100);
    btStream->print("AT+BAUD0"); // Ensure 9600 baud (0 = 9600)
    delay(100);
    btStream->print("AT+MODE0"); // Peripheral mode
    delay(100);
    */
}

void RobArt_BluetoothSerial::send(const String &message)
{
    if (btStream)
    {
        btStream->println(message);
    }
}

String RobArt_BluetoothSerial::receive() {
    String result = "";
    if (btStream && btStream->available()) {
        while (btStream->available()) {
            char c = btStream->read();
            Serial.println("read: " + c);
            if (c == '\n') break; // Stop at newline
            result += c;
            // No delay needed; rely on loop()'s timing
        }
    }else {
        Serial.println("Bluetooth stream not available");
    }
    return result;
}

bool RobArt_BluetoothSerial::available()
{
    return btStream && btStream->available();
}

Stream& RobArt_BluetoothSerial::getStream() {
    if (!btStream) {
        // Sicherstellen, dass btStream initialisiert wurde
        while (true) {
            Serial.println("Fehler: Bluetooth-Stream ist nicht initialisiert!");
            delay(1000);
        }
    }
    return *btStream;
}

// Wrapper-Methode
void RobArt_BluetoothSerial::println(const String &message) {
    if (btStream) {
        btStream->println(message);
    }
}


/* Weitere Ideen...

include <SoftwareSerial.h>

// Define HM-10 pins (RX of HM-10 to TX pin, TX of HM-10 to RX pin)
SoftwareSerial hm10(2, 3); // RX, TX
int counter = 0;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  // Start HM-10 communication at default baud rate
  hm10.begin(9600);
  
  Serial.println("HM-10 Peripheral Started");

  // Optional: Configure HM-10 with AT commands (only needed once)
  // Ensure HM-10 is not connected to any device before sending AT commands
  hm10.print("AT+NAMECounterBLE"); // Set device name
  delay(100);
  hm10.print("AT+BAUD0"); // Ensure 9600 baud (0 = 9600)
  delay(100);
  hm10.print("AT+MODE0"); // Peripheral mode
  delay(100);
}

void loop() {
  // Increment counter
  counter++;
  
  // Send counter value over HM-10
  hm10.println(counter);
  Serial.print("Sent Counter: ");
  Serial.println(counter);
  
  // Wait 2 seconds
  delay(2000);
}

*/