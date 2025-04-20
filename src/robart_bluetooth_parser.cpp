/******************************************************
 *
 * Project: RobArt the DrawingBot
 * Parser for Bluetooth Commands
 *
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 *
 ******************************************************/

#include "robart_bluetooth_parser.h"

RobArt_Bluetooth_Parser::RobArt_Bluetooth_Parser(Stream &stream) : btStream(stream) {}

void RobArt_Bluetooth_Parser::update() {
    if (btStream.available()) {
        // TODO geht das?, ggfs muss ich hier RobArt_BluetoothSerial übergeben und nicht nur den Stream.
        String command = btStream.readStringUntil('\n');
        command.trim();
        parseCommand(command);
    }
}

void RobArt_Bluetooth_Parser::onMove(MoveCallback cb) {
    moveCb = cb;
}

void RobArt_Bluetooth_Parser::onStatus(StatusCallback cb) {
    statusCb = cb;
}

void RobArt_Bluetooth_Parser::onLed(LedCallback cb) {
    ledCb = cb;
}

void RobArt_Bluetooth_Parser::parseCommand(const String &command) {
    if (command.startsWith("G1")) {
        int x = extractValue(command, 'X');
        int y = extractValue(command, 'Y');
        if (moveCb) moveCb(x, y);
        else moveTo(x, y);
    } else if (command.startsWith("M3")) {
        int pwm = extractValue(command, 'S');
        if (ledCb) ledCb(true, pwm);
        else {
            analogWrite(3, pwm); // Beispiel: MOTOR_PIN = 3
            digitalWrite(13, HIGH); // LED_PIN = 13
        }
    } else if (command.startsWith("M5")) {
        if (ledCb) ledCb(false, 0);
        else {
            analogWrite(3, 0);
            digitalWrite(13, LOW);
        }
    } else if (command.startsWith("M105")) {
        if (statusCb) statusCb();
        else btStream.println("OK T:24.5");
    }
}

int RobArt_Bluetooth_Parser::extractValue(const String &cmd, char key) {
    int idx = cmd.indexOf(key);
    if (idx == -1) return 0;
    int endIdx = cmd.indexOf(' ', idx);
    if (endIdx == -1) endIdx = cmd.length();
    return cmd.substring(idx + 1, endIdx).toInt();
}

void RobArt_Bluetooth_Parser::moveTo(int x, int y) {
    btStream.print("Moving to X:");
    btStream.print(x);
    btStream.print(" Y:");
    btStream.println(y);
    // Füge hier echten Bewegungscode hinzu
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