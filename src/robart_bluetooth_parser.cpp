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
#include "robart_bluetooth_parser.h"

RobArt_Parser::RobArt_Parser(Stream &stream) : btStream(stream) {}

void RobArt_Parser::update() {
    static String command = ""; // Static to retain data across calls
    if (btStream.available()) {
        char c = btStream.read();
        if (c == '\n') {
            Serial.println("parser.update() - Empfangene Command: " + command);
            command.trim();
            parseCommand(command);
            command = ""; // Reset for next command
        } else {
            command += c; // Build command incrementally
        }
    }
}

void RobArt_Parser::onMove(MoveCallback cb) {
    moveCb = cb;
}

void RobArt_Parser::onStatus(StatusCallback cb) {
    statusCb = cb;
}

void RobArt_Parser::onLed(LedCallback cb) {
    ledCb = cb;
}

void RobArt_Parser::parseCommand(const String &command) {
    if (command.startsWith("G1")) {
        int x = extractValue(command, 'X');
        int y = extractValue(command, 'Y');

        if (moveCb) moveCb(x, y);
        reply(x, y);

    } else if (command.startsWith("M3")) {
        int pwm = extractValue(command, 'S');
        if (ledCb) ledCb(true, pwm);
        else {
            //! analogWrite(3, pwm); // Beispiel: MOTOR_PIN = 3
            //! digitalWrite(13, HIGH); // LED_PIN = 13
        }
    } else if (command.startsWith("M5")) {
        if (ledCb) ledCb(false, 0);
        else {
            //! analogWrite(3, 0);
            //! digitalWrite(13, LOW);
        }

    } else if (command.startsWith("M105")) {
        if (statusCb) statusCb();
        else btStream.println("OK T:24.5");
    }
}

int RobArt_Parser::extractValue(const String &cmd, char key) {
    int idx = cmd.indexOf(key);
    if (idx == -1) return 0;
    int endIdx = cmd.indexOf(' ', idx);
    if (endIdx == -1) endIdx = cmd.length();
    return cmd.substring(idx + 1, endIdx).toInt();
}

void RobArt_Parser::reply(int x, int y) {
    btStream.print("Moving to X:");
    btStream.print(x);
    btStream.print(" Y:");
    btStream.println(y);
}





//! --- altes eug
/*
void RobArt_Bluetooth_Parser::parsePEN(String params)
{
    // sscanf(params.substring(index).c_str(), "%f %f", &pen_nr, &pen_command);
}

// in: path
// return: float &vx, float &vy, float &lastX, float &lastY (pointers to)
// TODO return values as struct, instead of using pointers?
void RobArt_Bluetooth_Parser::parseSVGPath(String path, float *vx, float *vy, float *lastX, float *lastY)
{

    float newX, newY, ctrlX1, ctrlY1, ctrlX2, ctrlY2;
    char command;
    int index = 0;

    while (index < path.length())
    {
        command = path[index];
        index++;

        if (command == 'M' || command == 'L')
        { // Move to or Line to
            sscanf(path.substring(index).c_str(), "%f %f", &newX, &newY);
            *vx = newX - *lastX;
            *vy = newY - *lastY;
            *lastX = newX;
            *lastY = newY;
        }
        else if (command == 'C')
        { // Cubic Bézier curve
            sscanf(path.substring(index).c_str(), "%f %f %f %f %f %f", &ctrlX1, &ctrlY1, &ctrlX2, &ctrlY2, &newX, &newY);
            *vx = newX - *lastX;
            *vy = newY - *lastY;
            *lastX = newX;
            *lastY = newY;
        }
        else if (command == 'Q')
        { // Quadratic Bézier curve
            sscanf(path.substring(index).c_str(), "%f %f %f %f", &ctrlX1, &ctrlY1, &newX, &newY);
            *vx = newX - *lastX;
            *vy = newY - *lastY;
            *lastX = newX;
            *lastY = newY;
        }
        else if (command == 'Z' || command == 'z')
        { // Close path
            vx = 0;
            vy = 0;
        }

        index = path.indexOf(' ', index) + 1;
    }
}
*/