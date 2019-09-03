/*

S7XG library

GPS auto report example
In this example the S7XG module reports GPS data via LoRaWAN every X seconds.

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

const char *devAddr = "26011433";
const char *nwkSKey = "5DE49A0F0C9649B8D466B9032DAAB331";
const char *appSKey = "EE0080DAB519CEF94E2EC83A110AA43A";

void setup() {

    // Reset the S7XG module
    #if defined(ARDUINO_T_WATCH)
        Wire.begin(21, 22);
        axp.begin(Wire);
        s7xg_power(false);
        delay(100);
        s7xg_power(true);
    #endif

    // Init connection to the PC
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println("[INFO ] S7XG basic GPS");
    Serial.println();

    // Init connection to the module
    SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);
    module.begin(SerialS7XG);

    // Transmit at max power ETSI allows
    module.macPower(14);

    // Use SF7 and BW125
    module.macDatarate(S7XG_DR_SF7BW125_EU);

    // Do not use ADR (moving device)
    module.macADR(false);

    // Initialize GPS (5s cycle, manual mode, gps system, hot start,...)
    module.gpsInit();

    // Report every 20 seconds
    module.txCycle(20);

    // On LoRaWAN port 3
    module.gpsPort(3);

    // Use CayenneLPP compatible encoding
    module.gpsFormat(S7XG_GPS_FORMAT_IPSO);

    // Join the network using Activation-By-Personalisation
    module.macJoinABP(devAddr, nwkSKey, appSKey);

    // Set GPS in auto mode
    module.gpsMode(S7XG_GPS_MODE_AUTO);

    // The module will now start to send GPS coordinates via LoRaWAN every X seconds.
    // If the GPS does not have a fix it will send a non-LLP valid payload

    // MAC and GPS properties can be changed on-the-fly.
    // The module will report new messagesusing the changed properties
    // until GPS mode is changed.

}

void loop() {
    delay(1);
}