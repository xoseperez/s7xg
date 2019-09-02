/*

S7XG library

Basic GPS example

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

#include "S7XG.h"

#ifndef ARDUINO_ARCH_ESP32
    #error "This scketch is meant to run on an ESP32 board"
#endif

HardwareSerial SerialS7XG(1);
S7XG module;

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
        delay(100);
        s7xg_power(true);
    #endif

    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println("[INFO ] S7XG basic GPS");
    Serial.println();

    SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);
    module.begin(SerialS7XG);
    module.gpsInit();

}

void loop() {
    
    delay(5000);
    
    gps_message_t message = module.gpsData();
    if (message.fix) {
        Serial.print("Latitude   : "); Serial.println(message.latitude, 6);
        Serial.print("Longitude  : "); Serial.println(message.longitude, 6);
        char buffer[40];
        snprintf(
            buffer, sizeof(buffer), "Datetime   : %04d-%02d-%02d %02d:%02d:%02d", 
            message.year, message.month, message.day,
            message.hour, message.minute, message.second);
        Serial.println(buffer);
    }
    Serial.print("Positioning: "); Serial.println(message.positioning);
    Serial.println();

}