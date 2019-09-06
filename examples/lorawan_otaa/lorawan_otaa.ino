/*

S7XG library

Join a LoRaWAN network in OTAA mode

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

HardwareSerial SerialS7XG(1);

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

const char *appEui = "70B3D57EF0001B38";
const char *appKey = "A8E60CE2CAB3BD95E3B9297E4C9E2909";

void setup() {
    
    // Reset the S7XG module power supply
    #if defined(ARDUINO_T_WATCH)
        Wire.begin(21, 22);
        axp.begin(Wire);
        s7xg_power(false);
        delay(1000);
        s7xg_power(true);
    #endif

    // Init PC connection
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println("[INFO ] LoRaWAN OTAA join example");
    Serial.println();

    // Init connection to the module
    SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);
    module.begin(SerialS7XG);

    // Show the Device EUI
    Serial.print  ("[INFO ] Device EUI: ");
    Serial.println(module.getEUI());

    // Transmit at max power ETSI allows
    module.macPower(14);

    // Use SF7 and BW125
    module.macDatarate(S7XG_DR_SF7BW125_EU);

    // Do not use ADR (moving device)
    module.macADR(false);

    // Disable duty cycle check (never do this in production!)
    module.macDutyCycle(false);

    module.macJoinOTAA(appEui, appKey);
    if (module.macWaitJoined()) {
        Serial.println("[INFO ] Joined!");
    } else {
        Serial.println("[ERROR] Timeout while joining");
        while (true) delay(1);
    }
    
    delay(1000);

}

void loop() {
    static uint8_t count = 0;
    uint8_t payload[1] = { count++ };
    Serial.print("[INFO ] Sending ");
    Serial.println(count);
    if (!module.macSend(payload, 1)) {
        Serial.print("[ERROR] Response: ");
        Serial.println(module.getResponse());
    }
    delay(10000);
}