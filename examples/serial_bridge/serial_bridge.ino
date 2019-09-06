/*

S7XG library

Simple serial bridge

Copyright (C) 2019 by Xose Pérez <xose at espurna dot io>

The S7XG library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The S7XG library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the S7XG library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef ARDUINO_ARCH_ESP32
    #error "This scketch is meant to run on an ESP32 board"
#endif

// This is the hardwareserial port connected to the S7XG module
HardwareSerial SerialS7XG(1);

// This is required for the TTGO-T-Watch
#if defined(ARDUINO_T_WATCH)

#include <Wire.h>
#include "axp20x.h"
AXP20X_Class axp;

void s7xg_power(bool status) {
    axp.setLDO4Voltage(AXP202_LDO4_1800MV);
    axp.setPowerOutPut(AXP202_LDO4, status ? AXP202_ON : AXP202_OFF);
}

#endif

void setup() {
    
    // Power the S7XG module
    #if defined(ARDUINO_T_WATCH)
        Wire.begin(21, 22);
        axp.begin(Wire);
        s7xg_power(false);
        delay(1000);
        s7xg_power(true);
    #endif

    // Init PC serial
    Serial.begin(115200);

    // Init module serial
    SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);

}

void loop() {

    if (Serial.available()) {
        SerialS7XG.write(Serial.read());
    }

    if (SerialS7XG.available()) {
        Serial.write(SerialS7XG.read());
    }

}