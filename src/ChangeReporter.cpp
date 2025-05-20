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

// Copyright (C) 2025 [Carsten Nichte]
// This file is part of Robart.
// Licensed under the GNU General Public License v3.0. See LICENSE for details.
#include "ChangeReporter.h"

void ChangeReporter::reportIfChanged(const String label, const String &newValue) {
    if (newValue != lastValue) {
        Serial.println(label + ": " + String(newValue));
        lastValue = newValue;
    }
}