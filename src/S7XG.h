/*

S7XG library

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

*/

#pragma once

#include <Arduino.h>

// ----------------------------------------------------------------------------
// Configuration
// ----------------------------------------------------------------------------

#define S7XG_SHORT_TIMEOUT                    300
#define S7XG_LONG_TIMEOUT                     5000
#define S7XG_RX_BUFFER_SIZE                   128
#define S7XG_TX_BUFFER_SIZE                   128

// ----------------------------------------------------------------------------
// Debug
// ----------------------------------------------------------------------------

#if defined(S7XG_DEBUG_SERIAL)
  #define S7XG_DEBUG(...) S7XG_DEBUG_SERIAL.print( __VA_ARGS__ )
#else
  #define S7XG_DEBUG(...) 
#endif

// ----------------------------------------------------------------------------
// LoRaWAN
// ----------------------------------------------------------------------------

enum {

  S7XG_DR_SF12BW125_EU = 0,
  S7XG_DR_SF11BW125_EU,
  S7XG_DR_SF10BW125_EU,
  S7XG_DR_SF9BW125_EU,
  S7XG_DR_SF8BW125_EU,
  S7XG_DR_SF7BW125_EU,
  S7XG_DR_SF7BW250_EU,

  S7XG_DR_SF12BW125_CN = 0,
  S7XG_DR_SF11BW125_CN,
  S7XG_DR_SF10BW125_CN,
  S7XG_DR_SF9BW125_CN,
  S7XG_DR_SF8BW125_CN,
  S7XG_DR_SF7BW125_CN,

  S7XG_DR_SF10BW125_US = 0,
  S7XG_DR_SF9BW125_US,
  S7XG_DR_SF8BW125_US,
  S7XG_DR_SF7BW125_US,
  S7XG_DR_SF8BW500_US,

};

enum {
  S7XG_MAC_CLASS_A = 'A',
  S7XG_MAC_CLASS_C = 'C',
};

// ----------------------------------------------------------------------------
// GPS
// ----------------------------------------------------------------------------

typedef struct {
  bool fix;
  float latitude;
  float longitude;
  float positioning;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} gps_message_t;

enum {
  S7XG_GPS_MODE_IDLE = 0,
  S7XG_GPS_MODE_MANUAL,
  S7XG_GPS_MODE_AUTO
};

enum {
  S7XG_GPS_FORMAT_RAW = 0,
  S7XG_GPS_FORMAT_IPSO,
  S7XG_GPS_FORMAT_KIWI,
  S7XG_GPS_FORMAT_UTC_POS,
};

enum {
  S7XG_GPS_START_HOT = 0,
  S7XG_GPS_START_WARM,
  S7XG_GPS_START_COLD,
};

enum {
  S7XG_GPS_SYSTEM_GPS = 0,
  S7XG_GPS_SYSTEM_HYBRID,
};

// ----------------------------------------------------------------------------
// Commands
// ----------------------------------------------------------------------------

const char SIP_FACTORY_RESET[] PROGMEM =          "sip factory_reset";              // 3.1.1
const char SIP_GET_VER[] PROGMEM =                "sip get_ver";                    // 3.1.2
const char SIP_RESET[] PROGMEM =                  "sip reset";                      // 3.1.3
const char SIP_GET_HW_MODEL[] PROGMEM =           "sip get_hw_model";               // 3.1.4
const char SIP_SET_ECHO[] PROGMEM =               "sip set_echo %s";                // 3.1.5
const char SIP_SET_LOG[] PROGMEM =                "sip set_log %s";                 // 3.1.6
const char SIP_SLEEP[] PROGMEM =                  "sip sleep %d uart_on";           // 3.1.7
const char SIP_SET_BAUDRATE[] PROGMEM =           "sip set_baudrate %d %s";         // 3.1.8
const char SIP_GET_HW_MODEL_VER[] PROGMEM =       "sip get_hw_model_ver";           // 3.1.9
const char SIP_SET_GPIO_MODE[] PROGMEM =          "sip set_gpio_mode %c %d %d";     // 3.1.10
const char SIP_SET_GPIO[] PROGMEM =               "sip set_gpio %c %d %d";          // 3.1.11
const char SIP_GET_GPIO[] PROGMEM =               "sip get_gpio %c %d";             // 3.1.12
const char SIP_GET_UUID[] PROGMEM =               "sip get_uuid";                   // 3.1.13
const char SIP_SET_STORAGE[] PROGMEM =            "sip set_storage %s";             // 3.1.14
const char SIP_GET_STORAGE[] PROGMEM =            "sip get_storage";                // 3.1.15
const char SIP_SET_BATT_RESISTOR[] PROGMEM =      "sip set_batt_resistor %lu %lu";  // 3.1.16
const char SIP_GET_BATT_RESISTOR[] PROGMEM =      "sip get_batt_resistor";          // 3.1.17
const char SIP_GET_BATT_VOLT[] PROGMEM =          "sip get_batt_volt";              // 3.1.18

const char MAC_TX[] PROGMEM =                     "mac tx %s %d %s";                // 3.2.1
const char MAC_JOIN_ABP[] PROGMEM =               "mac join abp";                   // 3.2.2
const char MAC_JOIN_OTAA[] PROGMEM =              "mac join otaa";                  // 3.2.2
const char MAC_SAVE[] PROGMEM =                   "mac save";                       // 3.2.3
const char MAC_GET_JOIN_STATUS[] PROGMEM =        "mac get_join_status";            // 3.2.4
const char MAC_SET_LINKCHK[] PROGMEM =            "mac set_linkchk";                // 3.2.5
const char MAC_SET_DEVEUI[] PROGMEM =             "mac set_deveui %s";              // 3.2.6
const char MAC_SET_APPEUI[] PROGMEM =             "mac set_appeui %s";              // 3.2.7
const char MAC_SET_APPKEY[] PROGMEM =             "mac set_appkey %s";              // 3.2.8
const char MAC_SET_DEVADDR[] PROGMEM =            "mac set_devaddr %s";             // 3.2.9
const char MAC_SET_NWKSKEY[] PROGMEM =            "mac set_nwkskey %s";             // 3.2.10
const char MAC_SET_APPSKEY[] PROGMEM =            "mac set_appskey %s";             // 3.2.11
const char MAC_SET_POWER[] PROGMEM =              "mac set_power %d";               // 3.2.12
const char MAC_SET_DR[] PROGMEM =                 "mac set_dr %d";                  // 3.2.13
const char MAC_SET_ADR[] PROGMEM =                "mac set_adr %s";                 // 3.2.14
const char MAC_SET_TXRETRY[] PROGMEM =            "mac set_txretry %d";             // 3.2.15
const char MAC_SET_RXDELAY1[] PROGMEM =           "mac set_rxdelay1 %d";            // 3.2.16
const char MAC_SET_RX2[] PROGMEM =                "mac set_rx2 %d %d";              // 3.2.17
const char MAC_SET_SYNC[] PROGMEM =               "mac set_sync %02X";              // 3.2.18
const char MAC_SET_CH_FREQ[] PROGMEM =            "mac set_ch_freq %d %d";          // 3.2.19
const char MAC_SET_CH_DR_RANGE[] PROGMEM =        "mac set_ch_dr_range %d %d %d";   // 3.2.20
const char MAC_SET_CH_STATUS[] PROGMEM =          "mac set_ch_status %d %s";        // 3.2.21
const char MAC_SET_DC_CTL[] PROGMEM =             "mac set_dc_ctl %s";              // 3.2.22
const char MAC_SET_DC_BAND[] PROGMEM =            "mac set_dc_band %d %d";          // 3.2.23
const char MAC_SET_JOIN_CH[] PROGMEM =            "mac set_join_ch %d %s";          // 3.2.24
const char MAC_SET_UPCNT[] PROGMEM =              "mac set_upcnt %lu";              // 3.2.25
const char MAC_SET_DOWNCNT[] PROGMEM =            "mac set_downcnt %lu";            // 3.2.26
const char MAC_SET_CLASS[] PROGMEM =              "mac set_class %c";               // 3.2.27
const char MAC_GET_DEVADDR[] PROGMEM =            "mac get_devaddr";                // 3.2.28
const char MAC_GET_DEVEUI[] PROGMEM =             "mac get_deveui";                 // 3.2.29
const char MAC_GET_APPEUI[] PROGMEM =             "mac get_appeui";                 // 3.2.30
const char MAC_GET_NWKSKEY[] PROGMEM =            "mac get_nwkskey";                // 3.2.31
const char MAC_GET_APPSKEY[] PROGMEM =            "mac get_appskey";                // 3.2.32
const char MAC_GET_APPKEY[] PROGMEM =             "mac get_appkey";                 // 3.2.33
const char MAC_GET_DR[] PROGMEM =                 "mac get_dr";                     // 3.2.34
const char MAC_GET_BAND[] PROGMEM =               "mac get_band";                   // 3.2.35
const char MAC_GET_POWER[] PROGMEM =              "mac get_power";                  // 3.2.36
const char MAC_GET_ADR[] PROGMEM =                "mac get_adr";                    // 3.2.37
const char MAC_GET_TXRETRY[] PROGMEM =            "mac get_txretry";                // 3.2.38
const char MAC_GET_RXDELAY[] PROGMEM =            "mac get_rxdelay";                // 3.2.39
const char MAC_GET_RX2[] PROGMEM =                "mac get_rx2";                    // 3.2.40
const char MAC_GET_SYNC[] PROGMEM =               "mac get_sync";                   // 3.2.41
const char MAC_GET_CH_PARA[] PROGMEM =            "mac get_ch_para %d";             // 3.2.42
const char MAC_GET_CH_STATUS[] PROGMEM =          "mac get_ch_status %d";           // 3.2.43
const char MAC_GET_DC_CTL[] PROGMEM =             "mac get_dc_ctl";                 // 3.2.44
const char MAC_GET_DC_BAND[] PROGMEM =            "mac get_dc_band %d";             // 3.2.45
const char MAC_GET_JOIN_CH[] PROGMEM =            "mac get_join_ch";                // 3.2.46
const char MAC_GET_UPCNT[] PROGMEM =              "mac get_upcnt";                  // 3.2.47
const char MAC_GET_DOWNCNT[] PROGMEM =            "mac get_downcnt";                // 3.2.48
const char MAC_GET_CLASS[] PROGMEM =              "mac get_class";                  // 3.2.49
const char MAC_SET_TX_MODE[] PROGMEM =            "mac set_tx_mode %s";             // 3.2.50
const char MAC_GET_TX_MODE[] PROGMEM =            "mac get_tx_mode";                // 3.2.51
const char MAC_SET_BATT[] PROGMEM =               "mac set_batt %d";                // 3.2.52
const char MAC_GET_BATT[] PROGMEM =               "mac get_batt";                   // 3.2.53
const char MAC_SET_TX_CONFIRM[] PROGMEM =         "mac set_tx_confirm %s";          // 3.2.54
const char MAC_GET_TX_CONFIRM[] PROGMEM =         "mac get_tx_confirm";             // 3.2.55
const char MAC_SET_LBT[] PROGMEM =                "mac set_lbt %s";                 // 3.2.56
const char MAC_GET_LBT[] PROGMEM =                "mac get_lbt";                    // 3.2.57
const char MAC_SET_UPLINK_DWELL[] PROGMEM =       "mac set_uplink_dwell %s";        // 3.2.58
const char MAC_GET_UPLINK_DWELL[] PROGMEM =       "mac get_uplink_dwell";           // 3.2.59
const char MAC_SET_DOWNLINK_DWELL[] PROGMEM =     "mac set_downlink_dwell %s";      // 3.2.60
const char MAC_GET_DOWNLINK_DWELL[] PROGMEM =     "mac get_downlink_dwell";         // 3.2.61
const char MAC_SET_MAX_EIRP[] PROGMEM =           "mac set_max_eirp %d";            // 3.2.62
const char MAC_GET_MAX_EIRP[] PROGMEM =           "mac get_max_eirp";               // 3.2.63
const char MAC_SET_CH_COUNT[] PROGMEM =           "mac set_ch_count %d %d";         // 3.2.64
const char MAC_GET_CH_COUNT[] PROGMEM =           "mac get_ch_count";               // 3.2.65
const char MAC_SET_KEYS[] PROGMEM =               "mac set_keys %s %s %s %s %s %s"; // 3.2.66
const char MAC_SET_TX_INTERVAL[] PROGMEM =        "mac set_tx_interval %lu";        // 3.2.67
const char MAC_GET_TX_INTERVAL[] PROGMEM =        "mac get_tx_interval";            // 3.2.68
const char MAC_SET_RX1_FREQ[] PROGMEM =           "mac set_rx1_freq %lu %lu %d";    // 3.2.69
const char MAC_GET_RX1_FREQ[] PROGMEM =           "mac get_rx1_freq";               // 3.2.70
const char MAC_SET_AUTO_JOIN[] PROGMEM =          "mac set_auto_join %s %s %d";     // 3.2.71
const char MAC_GET_AUTO_JOIN[] PROGMEM =          "mac get_auto_join";              // 3.2.72
const char MAC_SET_POWER_INDEX[] PROGMEM =        "mac set_power_index %d";         // 3.2.73
const char MAC_GET_POWER_INDEX[] PROGMEM =        "mac get_power_index";            // 3.2.74

const char GPS_SET_LEVEL_SHIFT[] PROGMEM =        "gps set_level_shift %s";         // 3.3.1
const char GPS_SET_NMEA[] PROGMEM =               "gps set_nmea %s";                // 3.3.2
const char GPS_SET_PORT_UPLINK[] PROGMEM =        "gps set_port_uplink %d";         // 3.3.3
const char GPS_SET_FORMAT_UPLINK[] PROGMEM =      "gps set_format_uplink %s";       // 3.3.4
const char GPS_SET_POSITIONING_CYCLE[] PROGMEM =  "gps set_positioning_cycle %lu";  // 3.3.5
const char GPS_SET_MODE[] PROGMEM =               "gps set_mode %s";                // 3.3.6
const char GPS_GET_MODE[] PROGMEM =               "gps get_mode";                   // 3.3.7
const char GPS_GET_DATA[] PROGMEM =               "gps get_data dd";                // 3.3.8
const char GPS_SLEEP_ON[] PROGMEM =               "gps sleep on %d";                // 3.3.9
const char GPS_SLEEP_OFF[] PROGMEM =              "gps sleep off";                  // 3.3.9
const char GPS_GET_TTFF[] PROGMEM =               "gps get_ttff";                   // 3.3.10
const char GPS_RESET[] PROGMEM =                  "gps reset";                      // 3.3.11
const char GPS_SET_SATELLITE_SYSTEM[] PROGMEM =   "gps set_satellite_system %s";    // 3.3.12
const char GPS_SET_START[] PROGMEM =              "gps set_start %s";               // 3.3.13

// ----------------------------------------------------------------------------
// Class definition
// ----------------------------------------------------------------------------

class S7XG {

  public:

    void begin(Stream &);

    void reset();
    bool sleep(uint32_t seconds);
    bool wake();
    char * getResponse();
    char * getVersion();
    char * getHardware();
    char * getEUI();

    // LoRaWAN
    bool macSend(char * data, bool confirmed = false, uint8_t port = 1);
    bool macSend(uint8_t * data, uint8_t len, bool confirmed = false, uint8_t port = 1);
    bool macJoinABP(const char * devaddr, const char * nwkskey, const char * appskey);
    bool macJoinOTAA(const char * deveui, const char * appeui, const char * appkey);
    bool macJoinOTAA(const char * appeui, const char * appkey);
    bool macSave();
    bool macJoined();
    bool macWaitJoined(uint32_t timeout = 10000);
    bool macPower(uint8_t power);
    bool macDatarate(uint8_t dr);
    bool macADR(bool adr);
    bool macRetries(uint8_t times);
    bool macSync(uint8_t sync);
    bool macChannelFrequency(uint8_t channel, uint32_t frequency);
    bool macChannelStatus(uint8_t channel, bool status);
    bool macDutyCycle(bool dc);
    bool macUpCounter(uint32_t counter);
    bool macDownCounter(uint32_t counter);
    bool macClass(uint8_t value);
    uint16_t macBand();
    uint32_t macUpCounter();
    uint32_t macDownCounter();
    bool txCycle(uint32_t seconds);

    // GPS
    bool gpsInit();
    bool gpsPort(uint8_t port);
    bool gpsFormat(uint8_t format);
    bool gpsCycle(uint32_t seconds);
    bool gpsMode(uint8_t mode);
    uint8_t gpsMode();
    gps_message_t gpsData();
    bool gpsSleep(bool deep);
    bool gpsWake();
    bool gpsReset();
    bool gpsSystem(uint8_t system);
    bool gpsStart(uint8_t mode);

    // Utils
    char * hexlify(uint8_t * source, char * destination, uint8_t len);
    uint8_t * unhexlify(char * source, uint8_t * destination, uint8_t len);

  protected:

    void _flush();
    template<typename T> void _send(T * s);
    template<typename T> char * _sendAndReturn(T * s);
    bool _sendAndACK(PGM_P format_P, ...);

    uint8_t _readLine();
    uint8_t _nibble(char ch);
    void _nice_delay(uint32_t ms);

    Stream *_stream;
    bool _wait_longer = false;
    char _buffer[S7XG_RX_BUFFER_SIZE];
    char _eui[17] = {0};

};
