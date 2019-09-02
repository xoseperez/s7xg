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

// ----------------------------------------------------------------------------

Module functions as per datasheet (included in the docs folder of the repo).

*/

#include "S7XG.h"

// ----------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------

/**
 * @brief               Binds the library the the stream object
 * @param &stream       Serial object to communicate with the S7XG module
 */
void S7XG::begin(Stream &stream) {
    _stream = &stream;
}

/**
 * @brief               Returns a pointer to the response buffer you can use to get the last error 
 * @return              Pointer to the response buffer (c-string)
 */
char * S7XG::getResponse() {
    return _buffer;
}

// ----------------------------------------------------------------------------
// SIP
// ----------------------------------------------------------------------------

/**
 * @brief               Gets the S7XG module version
 * @return              Pointer to a C-string containing the version
 */
char * S7XG::getVersion() {
    return _sendAndReturn(SIP_GET_VER);
}

/**
 * @brief               Resets the S7XG module
 */
void S7XG::reset() {
    _send(SIP_RESET);
    _nice_delay(250);
    while (!_stream->available()) delay(1);
    _flush();
}

/**
 * @brief               Gets the S7XG module hardware version
 * @return              Pointer to a C-string containing the hardware version
 */
char * S7XG::getHardware() {
    return _sendAndReturn(SIP_GET_HW_MODEL);
}

/**
 * @brief               Sets the S7XG in sleep mode for a number of seconds
 * @param[in] seconds   Seconds to sleep (must be a multiple of 10)
 * @return              True if everything OK
 */
bool S7XG::sleep(uint32_t seconds) {
    char command[32];
    snprintf_P(command, sizeof(command), SIP_SLEEP, seconds);
    _send(command);
    _readLine();
    return (_buffer == strstr(_buffer, "sleep"));
}

/**
 * @brief               Wakes the S7XG from sleep
 * @return              True if everything OK
 */
bool S7XG::wake() {
    return _sendAndACK(SIP_GET_VER);
}

/**
 * @brief               Returns a device EUI based on the S7XG module unique ID
 * @return              Pointer to a c-string with the HEX representation of the EUI
 */
char * S7XG::getEUI() {
    
    if (0 == _eui[0]) {
        
        _send(SIP_GET_UUID);
        _readLine();
        
        if (_buffer == strstr(_buffer, "uuid=")) {
            
            uint8_t uuid[12];
            unhexlify(&_buffer[5], uuid, 12);

            uint8_t eui[8];
            eui[0] = uuid[1];
            eui[1] = uuid[3];
            eui[2] = uuid[4];
            eui[3] = uuid[7];
            eui[4] = uuid[8];
            eui[5] = uuid[9];
            eui[6] = uuid[10];
            eui[7] = uuid[11];

            hexlify(eui, _eui, 8);

        }

    }

    return _eui;

}

// ----------------------------------------------------------------------------
// MAC
// ----------------------------------------------------------------------------

/**
 * @brief               Sends a byte array as a LoRaWAN message
 * @param[in] data      Byte array with the data to send
 * @param[in] len       Length of the byte array
 * @param[in] confirmed True to send a message with ACK request (defaults to false)
 * @param[in] port      LoRaWAN port (defaults to 1)
 * @return              True if everything OK
 */
bool S7XG::macSend(uint8_t * data, uint8_t len, bool confirmed, uint8_t port) {
    char hex[len * 2 + 1];
    return _sendAndACK(MAC_TX, confirmed ? "cnf" : "ucnf", port, hexlify(data, hex, len));
}

/**
 * @brief               Sends a c-string as a LoRaWAN message
 * @param[in] data      C-string to send
 * @param[in] confirmed True to send a message with ACK request (defaults to false)
 * @param[in] port      LoRaWAN port (defaults to 1)
 * @return              True if everything OK
 */
bool S7XG::macSend(char * data, bool confirmed, uint8_t port) {
    return macSend((uint8_t *) data, strlen(data));
}

/**
 * @brief               Joins a LoRaWAN network in ABP mode
 * @param[in] devaddr   Device address (hex string representing 4 bytes)
 * @param[in] nwkskey   Network session key (hex string representing 16 bytes)
 * @param[in] appskey   Application session key (hex string representing 16 bytes)
 * @return              True if everything OK
 */
bool S7XG::macJoinABP(const char * devaddr, const char * nwkskey, const char * appskey) {
    
    if (!_sendAndACK(MAC_SET_DEVADDR, devaddr)) return false;
    if (!_sendAndACK(MAC_SET_NWKSKEY, nwkskey)) return false;
    if (!_sendAndACK(MAC_SET_APPSKEY, appskey)) return false;

    _send(MAC_JOIN_ABP);
    _readLine(5000);
    if (0 != strcmp(_buffer, "Ok")) return false;
    _readLine();
    return (0 == strcmp(_buffer, "accepted"));

}

/**
 * @brief               Joins a LoRaWAN network in OTAA mode
 * @param[in] deveui    Device EUI (hex string representing 8 bytes)
 * @param[in] appeui    Application EUI (hex string representing 8 bytes)
 * @param[in] appkey    Application key (hex string representing 16 bytes)
 * @return              True if everything OK
 */
bool S7XG::macJoinOTAA(const char * deveui, const char * appeui, const char * appkey) {
    
    if (!_sendAndACK(MAC_SET_DEVEUI, deveui)) return false;
    if (!_sendAndACK(MAC_SET_APPEUI, appeui)) return false;
    if (!_sendAndACK(MAC_SET_APPKEY, appkey)) return false;

    _send(MAC_JOIN_OTAA);
    _readLine(5000);
    if (0 != strcmp(_buffer, "Ok")) return false;
    _readLine(10000);
    return (0 == strcmp(_buffer, "accepted"));

}

/**
 * @brief               Joins a LoRaWAN network in OTAA mode using the hardware EUI
 * @param[in] appeui    Application EUI (hex string representing 8 bytes)
 * @param[in] appkey    Application key (hex string representing 16 bytes)
 * @return              True if everything OK
 */
bool S7XG::macJoinOTAA(const char * appeui, const char * appkey) {
    return macJoinOTAA((const char *) getEUI(), appeui, appkey);
}

/**
 * @brief               Saves LoRaWAN configuration parameters to flash
 * @return              True if everything OK
 */
bool S7XG::macSave() {
    return _sendAndACK(MAC_SAVE);
}

/**
 * @brief               Sets the transmission power.
 * @details             According to datasheet: "it can be 2, 5, 8, 11, 14, 20 (non-915 band); 
 *                      30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10 (915 band); 1
 *                      7, 16, 14, 12, 10, 7, 5, 2 (470 band)". 
 *                      Any value not supported by the current frequency band will result in an error.
 *                      Note that some values might not be legal in your region.
 * @param[in] power     Transmission power
 * @return              True if everything OK
 */
bool S7XG::macPower(uint8_t power) {
    return _sendAndACK(MAC_SET_POWER, power);
}

/**
 * @brief               Sets the datarate.
 * @param[in] dr        Datarate
 * @return              True if everything OK
 */
bool S7XG::macDatarate(uint8_t dr) {
    return _sendAndACK(MAC_SET_DR, dr);
}

/**
 * @brief               Enables the Adaptative Data Rate
 * @param[in] adr       True or false
 * @return              True if everything OK
 */
bool S7XG::macADR(bool adr) {
    return _sendAndACK(MAC_SET_ADR, adr ? "on" : "off");
}

/**
 * @brief               Enables or disable a certain channel
 * @param[in] channel   Channel ID (depends on band)
 * @param[in] status    True or false
 * @return              True if everything OK
 */
bool S7XG::macChannelStatus(uint8_t channel, bool status) {
    return _sendAndACK(MAC_SET_CH_STATUS, channel, status ? "on" : "off");
}

/**
 * @brief               Enables or disable the duty cycle check
 * @param[in] status    True or false
 * @return              True if everything OK
 */
bool S7XG::macDutyCycle(bool dc) {
    return _sendAndACK(MAC_SET_DC_CTL, dc ? "on" : "off");
}

/**
 * @brief               Returns the current frequency plan
 * @return              Current band (470, 868, 915 or 923)
 */
uint16_t S7XG::macBand() {
    return atol(_sendAndReturn(MAC_GET_BAND));
}

// ----------------------------------------------------------------------------
// GPS
// ----------------------------------------------------------------------------

/**
 * @brief               Inits the GPS into manual mode
 * @return              True if everything OK
 */
bool S7XG::gpsInit() {
    if (!_sendAndACK(GPS_SET_LEVEL_SHIFT, "on")) return false;
    if (!_sendAndACK(GPS_SET_START, "hot")) return false;
    if (!_sendAndACK(GPS_SET_SATELLITE_SYSTEM, "gps")) return false;
    if (!_sendAndACK(GPS_SET_NMEA, "rmc")) return false;
    if (!_sendAndACK(GPS_SET_MODE, "manual")) return false;
    return true;
}

/**
 * @brief               Gets the GPS mode
 * @param[in] mode      One of S7XG_GPS_MODE_IDLE, S7XG_GPS_MODE_MANUAL or S7XG_GPS_MODE_AUTO
 * @return              True if everything OK
 */
bool S7XG::gpsMode(uint8_t mode) {
    
    return _sendAndACK(GPS_SET_MODE, 
        mode == S7XG_GPS_MODE_IDLE ? "idle" : 
        mode == S7XG_GPS_MODE_MANUAL ? "manual" : 
        "auto");
    
}

/**
 * @brief               Gets the data from the GPS in manual mode
 * @return              gps_message_t object with the data
 */
gps_message_t S7XG::gpsData() {
    
    char * buffer = _sendAndReturn(GPS_GET_DATA);
    gps_message_t message;

    // No data yet:
    // POSITIONING ( 14.8s )
    // GPS data OK:
    // DD UTC( 2019/09/02 12:33:34 ) LAT( 41.601215 N ) LONG( 2.622485 E ) POSITIONING( 3.6s )

    // Tokenize
    char * tok = strtok(buffer, " ");
    message.fix = (0 == strcmp(tok, "DD"));
    tok = strtok(NULL, " "); // UTC( or (
    if (!tok) return message;
    
    if (message.fix) {
        
        char * date = strtok(NULL, " "); // date
        char * time = strtok(NULL, " "); // time
        sscanf(date, "%d/%d/%d", &message.year, &message.month, &message.day);
        sscanf(time, "%d:%d:%d", &message.hour, &message.minute, &message.second);

        strtok(NULL, " "); // )
        strtok(NULL, " "); // LAT(
        tok = strtok(NULL, " "); // latitude
        message.latitude = atof(tok);
        tok = strtok(NULL, " "); // latitude sign
        if ('S' == tok[0]) message.latitude = -message.latitude;
        strtok(NULL, " "); // )
        strtok(NULL, " "); // LONG(
        tok = strtok(NULL, " "); // longitude
        message.longitude = atof(tok);
        tok = strtok(NULL, " "); // longitude sign
        if ('W' == tok[0]) message.longitude = -message.longitude;
        strtok(NULL, " "); // )
        strtok(NULL, " "); // POSITIONING(

    }

    tok = strtok(NULL, " "); // positioning value
    message.positioning = atof(tok);

    
    return message;

}

/**
 * @brief               Sets the GPS in sleep mode
 * @param[in] deep      True to set it to deep sleep
 * @return              True if everything OK
 */
bool S7XG::gpsSleep(bool deep) {
    return _sendAndACK(GPS_SLEEP_ON, deep ? 1 : 0);
}

/**
 * @brief               Wakes the GPS from sleep mode
 * @return              True if everything OK
 */
bool S7XG::gpsWake() {
    return _sendAndACK(GPS_SLEEP_OFF);
}

// ----------------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------------

/**
 * @brief                   Turns an hexa-string into a byte array
 * @param[in] source        Hexa-string like "013D45"
 * @param[out] destination  Byte array to store the values to (must have len positions), { 0x01, 0x3D, 0x45 } for the example above
 * @param[in] len           Size of the byte array, which is half the size of the hexa-string
 * @return                  Pointer to the destination array
 */
uint8_t * S7XG::unhexlify(char * source, uint8_t * destination, uint8_t len) {
    for (uint8_t i=0; i<len; i++) {
        destination[i] = (_nibble(source[i*2]) << 4) + (_nibble(source[i*2+1]));
    }
    return destination;
}

/**
 * @brief                   Turns a byte array into and hexa-string
 * @param[in] source        Byte array to store the values to, { 0x01, 0x3D, 0x45 } for the example above
 * @param[out] destination  Hexa-string like "013D45" (must have len*2 positions)
 * @param[in] len           Size of the byte array
 * @return                  Pointer to the destination
 */
char * S7XG::hexlify(uint8_t * source, char * destination, uint8_t len) {
    for (uint8_t i=0; i<len; i++) {
        snprintf(&destination[i*2], 3, "%02X", source[i]);
    }
    return destination;
}

// ----------------------------------------------------------------------------
// Private
// ----------------------------------------------------------------------------

/**
 * @brief               Flushes the serial line
 */
void S7XG::_flush() {
    while (_stream->available()) _stream->read();
}

/**
 * @brief               Reads a line from the module
 * @details             Stores in the internal buffer from the first ">> " to the next 0x0A.
 * @param[in] timeout   Times out after this milliseconds
 * @return              Number of characters in the buffer
 */
uint8_t S7XG::_readLine(uint32_t timeout) {

    _buffer[0] = 0;
    uint8_t pointer = 0;
    uint8_t flag = 0;
    uint32_t start = millis();

    while (millis() - start < timeout) {
        if (_stream->available()) {
            uint8_t ch = _stream->read();

            #if defined(S7XG_DEBUG_SERIAL)
                if ((31 < ch) && (ch < 127)) {
                    char ch_buff[6];
                    snprintf(ch_buff, sizeof(ch_buff), "%c", ch);
                    S7XG_DEBUG(ch_buff);
                }
            #endif
            
            if (flag > 2) {
                if (0x0A == ch) break;
                _buffer[pointer++] = ch;
                _buffer[pointer] = 0;
                if (S7XG_RX_BUFFER_SIZE == pointer - 1) break;
            } else if (flag == 2) {
                flag = (' ' == ch) ? flag + 1 : 0;
            } else {
                flag = ('>' == ch) ? flag + 1 : 0;
            }
        }
    }
    S7XG_DEBUG(F("\n"));

    return pointer;

}

/**
 * @brief               Sends a C-string to the module
 * @param[in] s         Command to send
 */
template<typename T> void S7XG::_send(T * s) {
    S7XG_DEBUG(F("<< ")); S7XG_DEBUG(s); S7XG_DEBUG(F("\n"));
    _stream->print(s);
}

/**
 * @brief               Sends a C-string to the module and returns a pointer to the answer
 * @param[in] s         Command to send
 * @return              Pointer to the internal buffer with the answer
 */
template<typename T> char * S7XG::_sendAndReturn(T * s) {
    _flush();
    _send(s);
    _readLine();
    return _buffer;
}

/**
 * @brief               Builds and sends a command to the module
 * @param[in] format_P  PROGMEM format string
 * @param[in] ...       Any values to set the placeholders to
 * @return              True if the module answered "Ok"
 */
bool S7XG::_sendAndACK(PGM_P format_P, ...) {

    char format[strlen_P(format_P) + 1];
    memcpy_P(format, format_P, sizeof(format));

    va_list args;
    va_start(args, format_P);
    char command[S7XG_TX_BUFFER_SIZE];
    int len = vsnprintf(command, sizeof(command), format, args);
    va_end(args);

    if (len < S7XG_TX_BUFFER_SIZE) {
        _flush();
        _send(command);
        _readLine();
        return 0 == strcmp(_buffer, "Ok");
    }
    return false;

}

/**
 * @brief               Returns the decimal value for an alphanumeric value
 * @param[in] ch        Alfanumeric value [0-9a-fA-F]
 * @return              Decimal value or 0 if error
 */
uint8_t S7XG::_nibble(char ch) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if ('a' <= ch && ch <= 'f') return ch - 'a' + 10;
    if ('A' <= ch && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

/**
 * @brief                   Non-blocking delay
 * @param[in] ms            Milliseconds to delay
 */
void S7XG::_nice_delay(uint32_t ms) {
    uint32_t start = millis();
    while (millis() - start < ms) delay(1);
}