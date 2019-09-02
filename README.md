# S7XG Library

Arduino Library for the AcSIP S76G and S78G LoRaWAN (and GPS) chips.
It will probably also work with the S76S and S78S (only LoRaWAN) chips.

[![version](https://img.shields.io/badge/version-0.1.0-brightgreen.svg)](CHANGELOG.md)
[![codacy](https://img.shields.io/codacy/grade/7dfaab5c9ca44b3191f47d29cc2b99fa/master.svg)](https://www.codacy.com/app/xoseperez/s7xg/dashboard)
[![travis](https://travis-ci.org/xoseperez/s7xg.svg?branch=master)](https://travis-ci.org/xoseperez/s7xg)
[![license](https://img.shields.io/github/license/xoseperez/s7xg.svg)](LICENSE)
<br />
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=xose%2eperez%40gmail%2ecom&lc=US&no_note=0&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHostedGuest)
[![twitter](https://img.shields.io/twitter/follow/xoseperez.svg?style=social)](https://twitter.com/intent/follow?screen_name=xoseperez)

## Description

The [S7XG chips by AcSIP](http://www.acsip.com.tw/index.php?action=products&fid1=19) are high integrated chips with LoRaWAN and GPS (only the G series).

The S76G/S78G integrates a Semtech SX1276 and a 32-bit ultra-low power Cortex M0+ MCU (STM32L073x), S76G supports global 868 MHz or 915 MHz ISM-Bands. (S78G supports 433MHz or 470 MHz by using SX1278 and the identical MCU with S76G). Besides, S76G/S78G contains a GPS chip – SONY CXD5603GF which is used to receive GPS/GPS+GLONASS signals for positioning.

## Dependencies

Some examples are meant to be run on the [TTGO T-Watch]("http://s.click.aliexpress.com/e/mhONXf7S) [Aliexpress] with the [LoRa/GPS addon](http://s.click.aliexpress.com/e/oAJfgzJE) [Aliexpress]. These examples need the AXP202X library to manage the power management chip inside the watch and power the S7XG module.

## API Reference

The `S7XG` class enables Arduino devices to interface the S7XG module using the manufacturer command set. Check the command set reference in the `datasheet` folder.
The class is documented inline and the documentation is generated using [doxygen](http://www.doxygen.nl/) and stored in the `docs` folder.

## Examples

### Sending LPP-encoded payload to The Things Network using Activation-by-Personalisation

This example uses the CayenneLPP library. There are several sources for this library but I recommend the ElectronicCats version here: https://github.com/ElectronicCats/CayenneLPP.

```c

#include "S7XG.h"
#include "CayenneLPP.h"

HardwareSerial SerialS7XG(1);
CayenneLPP lpp(32);
S7XG s7xg;

// LoRaWAN credentials as copied from TTN
const char *devAddr = "26011433";
const char *nwkSKey = "5DE49A0F0C9649B8D466B9032DAAB331";
const char *appSKey = "EE0080DAB519CEF94E2EC83A110AA43A";

void setup() {
  
  // Init the hardware serial (this is ESP32 specific)
  SerialS7XG.begin(115200, SERIAL_8N1, 34, 33);
  
  // Init the S7XG module
  s7xg.begin(SerialS7XG);
  s7xg.macPower(14);
  s7xg.macDatarate(DATARATE_EU_SF7BW125);
  s7xg.macADR(false);

  // Join the network in ABP mode
  s7xg.macJoinABP(devAddr, nwkSKey, appSKey);

}

void loop() {

  // Build the payload using CayenneLPP
  lpp.reset();
  lpp.addTemperature(1, 22.5);
  lpp.addBarometricPressure(2, 1073.21);
  lpp.addGPS(3, 52.37365, 4.88650, 2);

  // Sending the data
  s7xg.macSend(lpp.getBuffer(), lpp.getSize());

  // Interval between messages
  delay(30000);

}
```

There are more examples in the `examples` folder.

## License

Copyright (C) 2019 by Xose Pérez <xose dot perez at gmail dot com>

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
