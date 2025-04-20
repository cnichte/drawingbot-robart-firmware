#ifndef CHANGEREPORTER_H
#define CHANGEREPORTER_H

#include <Arduino.h>

class ChangeReporter {
public:
    ChangeReporter() = default;
    void reportIfChanged(const String &newValue);

private:
    String lastValue = "";
};

#endif // CHANGEREPORTER_H