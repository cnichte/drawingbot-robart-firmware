/******************************************************
 * 
 * Project: RobArt the DrawingBot
 *  
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 * 
 ******************************************************/

#ifndef ROBART_BLUETOOTH_SERIAL_H
#define ROBART_BLUETOOTH_SERIAL_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class RobArt_BluetoothSerial
{
public:
    RobArt_BluetoothSerial(uint8_t rxPin, uint8_t txPin); // Constructor takes RX and TX pin numbers
    void begin(long baudRate = 9600);                     // Start SoftwareSerial with default 9600 baud
    void send(const String &message);
    String receive();
    bool available();
    Stream& getStream();
    void println(const String &message);

private:
    uint8_t rx, tx;
    SoftwareSerial *btStream;
};

#endif