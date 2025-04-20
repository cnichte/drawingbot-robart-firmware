#include "ChangeReporter.h"

void ChangeReporter::reportIfChanged(const String &newValue) {
    if (newValue != lastValue) {
        Serial.println(newValue);
        lastValue = newValue;
    }
}