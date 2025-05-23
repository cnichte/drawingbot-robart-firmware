/******************************************************
 *
 * Project: RobArt the DrawingBot
 *
 * written by Carsten Nichte
 * https://carsten-nichte.de/docs/drawingbot/
 *
 ******************************************************/

#ifndef ROBART_BLUETOOTH_PARSER_H
#define ROBART_BLUETOOTH_PARSER_H

#include <Arduino.h>

class RobArt_Parser
{
public:
    using MoveCallback = void (*)(int x, int y);
    using PenCallback = void (*)(uint8_t penId);
    using PenControlCallback = void (*)(bool lift, float value); // true = down, false = up; value = angle or PWM
    using ModeCallback = void (*)(uint8_t mode); // For M100 S<mode>

    using StatusCallback = void (*)();
    using LedCallback = void (*)(bool on, int pwm);

    RobArt_Parser(Stream &stream);
    void update();

    void onMove(MoveCallback cb);
    void onPenSelect(PenCallback cb);
    void onPenControl(PenControlCallback cb);
    void onMode(ModeCallback cb);

    void onStatus(StatusCallback cb);
    void onLed(LedCallback cb);
    
private:
    Stream &btStream;
    
    MoveCallback moveCb = nullptr;
    PenCallback penCb = nullptr;
    PenControlCallback penControlCb = nullptr; 
    ModeCallback modeCb = nullptr; 
    
    StatusCallback statusCb = nullptr;
    LedCallback ledCb = nullptr;

    void parseCommand(const String &command);
    int extractValue(const String &cmd, char key);
    void reply(int x, int y);
};

#endif // ROBART_BLUETOOTH_PARSER_H

/*
    void parsePEN(String params);
    void parseSVGPath(String path, float *vx, float *vy, float *lastX, float *lastY);
*/