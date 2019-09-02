/*

S7XG library

Basic module information,
shows how to connect using different platforms

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

#if defined(ARDUINO_ARCH_ESP32) 
    HardwareSerial SerialS7XG(1);
#elif defined(ARDUINO_ARCH_ESP8266) 
    #include "SoftwareSerial.h"
    SoftwareSerial SerialS7XG(4, 5);
#elif defined(ARDUINO_AVR_LEONARDO) 
    #define SerialS7XG Serial1
#elif defined(ARDUINO_ARCH_AVR)
    #include "SoftwareSerial.h"
    SoftwareSerial SerialS7XG(4, 5);
#endif

#include "S7XG.h"
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
        s7xg_power(true);
    #endif

    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println("S7XG Module Info");
    Serial.println("================");
    Serial.println();

    #if defined(ARDUINO_ARCH_ESP32) 
        SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);
    #elif defined (ARDUINO_ARCH_ESP8266) 
        SerialS7XG.begin(115200);
    #elif defined (ARDUINO_AVR_LEONARDO) 
        SerialS7XG.begin(115200);
    #elif defined (ARDUINO_ARCH_AVR) 
        SerialS7XG.begin(115200);
    #endif
    module.begin(SerialS7XG);
    //module.reset();

    Serial.print("Hardware: "); Serial.println(module.getHardware());
    Serial.print("Version : "); Serial.println(module.getVersion());
    Serial.print("EUI     : "); Serial.println(module.getEUI());
    Serial.print("Band    : "); Serial.println(module.macBand());

}

void loop() {
    delay(1);
}