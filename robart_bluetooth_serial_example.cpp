#include "robart_drive.h"
#include "robart_bluetooth_serial.h"
#include "robart_bluetooth_parser.h"

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

// Bluetooth Modul an RX=2, TX=3
RobArt_BluetoothSerial BluetoothSerial(10, 11);
RobArt_Bluetooth_Parser Parser = RobArt_Bluetooth_Parser();

// RobArt_Drive Instanz to manage the three stepper motors
RobArt_Drive Drive = RobArt_Drive(motorInterfaceType, stepPin1, dirPin1, stepPin2, dirPin2, stepPin3, dirPin3);

// Stift-Ansteuerung
char pen_nr;
char pen_command;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // Warte auf Verbindung (bei manchen Boards nötig)
  Serial.println("Hallo Serial Monitor!");

  BluetoothSerial.begin(); // Defaults to 9600 baud
  Serial.println("Bluetooth initialized");

  Drive.setup(1000); // max 4000
}

void loop()
{
  // put your main code here, to run repeatedly:
  static String b_last = "";                                      // Speichert die letzte ausgegebene Meldung
  String b_aktuell = BluetoothSerial.available() ? "ja" : "nein"; // Funktion, die die aktuelle Meldung liefert

  // Nur ausgeben, wenn sich die Meldung geändert hat
  if (b_aktuell != b_last)
  {
    Serial.println(b_aktuell);
    b_last = b_aktuell; // Aktualisiere die letzte Meldung
  }

  if (BluetoothSerial.available())
  {
    /*
        Serial.println("Bluetooth verfügbar");
        String msg = Serial.readStringUntil('\n');
        // String msg = BluetoothSerial.receive();
        Serial.print("Empfangene Daten: ");
        Serial.println(msg);
        BluetoothSerial.send("Echo: " + msg);
    */

    /*
        //! Testweise weitere Verarbeitung...
        if (msg.startsWith("PATH"))
        {
          Parser.parseSVGPath(msg.substring(5), &Drive.vx, &Drive.vy, &Drive.lastX, &Drive.lastY);
        }
        else if (msg.startsWith("PEN"))
        {
          // PEN 1|2|3|4|5|ALL UP|DOWN|TOGGLE
          Parser.parsePEN(msg.substring(4));
        }
        else
        {
          sscanf(msg.c_str(), "%f %f %f", &Drive.vx, &Drive.vy, &Drive.omega);
        }
    */
  }

  Drive.update(0);

  // delay(10);
}

//!  OLD UNTESTED STUFF - siehe auch robart_bluetooth_serial_example.cpp

/*

  if (static_cast<unsigned int>(Bluetooth.available()))
  {
    String input = Bluetooth.readStringUntil('\n');
    if (input.startsWith("PATH"))
    {
      Communication.parseSVGPath(input.substring(5), &Drive.vx, &Drive.vy, &Drive.lastX, &Drive.lastY);
    }
    else if (input.startsWith("PEN"))
    {
      // PEN 1|2|3|4|5|ALL UP|DOWN|TOGGLE
      Communication.parsePEN(input.substring(4));
    }
    else
    {
      sscanf(input.c_str(), "%f %f %f", &Drive.vx, &Drive.vy, &Drive.omega);
    }
  }

*/












------------------------------------------------------------










//* CODE BEISPIEL - LED an und ausschalten über Bluetooth

// In der iOS-App "LED_ON", "LED_OFF" oder "STATUS" senden.
// Rückantwort erscheint im Textbereich.

// HM-10	Arduino UNO
// VCC	5V (wenn Modul 5V-kompatibel, sonst 3.3V)
// GND	GND
// TXD	D2 (Software RX)
// RXD	D3 (Software TX, über Spannungsteiler auf 3.3V!)

//! Baudrate am Arduino auf 9600 setzen (Standard beim HM-10).
//! TX/RX-Pins richtig verbinden (Arduino TX → HM-10 RX, und umgekehrt).
//! Achtung auf Spannung: HM-10 oft nur 3.3V-tolerant!

#include <Arduino.h>
#include <SoftwareSerial.h>

#define RX_PIN 2
#define TX_PIN 3
SoftwareSerial BTSerial(RX_PIN, TX_PIN); // RX, TX

#define LED_PIN 13

String inputString = "";
bool stringComplete = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  BTSerial.begin(9600);  // Standard HM-10 Baudrate

  Serial.println("HM-10 BLE Ready");
  inputString.reserve(50); // Buffer für eingehenden Text
}

void loop() {
  // Eingehende Zeichen verarbeiten
  while (BTSerial.available()) {
    char inChar = (char)BTSerial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  // Wenn eine komplette Nachricht empfangen wurde
  if (stringComplete) {
    inputString.trim();  // \n und \r entfernen
    Serial.print("Empfangen: ");
    Serial.println(inputString);

    handleCommand(inputString);

    inputString = "";
    stringComplete = false;
  }
}

void handleCommand(String cmd) {
  if (cmd.equalsIgnoreCase("LED_ON")) {
    digitalWrite(LED_PIN, HIGH);
    BTSerial.println("LED an");
  } else if (cmd.equalsIgnoreCase("LED_OFF")) {
    digitalWrite(LED_PIN, LOW);
    BTSerial.println("LED aus");
  } else if (cmd.equalsIgnoreCase("STATUS")) {
    BTSerial.println(digitalRead(LED_PIN) == HIGH ? "LED ist EIN" : "LED ist AUS");
  } else {
    BTSerial.println("Befehl unbekannt");
  }
}
