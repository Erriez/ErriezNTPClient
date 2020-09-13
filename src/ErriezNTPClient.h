/*
 * MIT License
 *
 * Copyright (c) 2020 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \file ErriezNTPClient.h
 * \brief NTP client library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezNTPClient
 *      Documentation:  https://erriez.github.io/ErriezNTPClient
 */

#ifndef ERRIEZ_NTP_CLIENT_H_
#define ERRIEZ_NTP_CLIENT_H_

#include <Arduino.h>
#include <time.h>

#if defined(ARDUINO_ARCH_AVR)
#include <EthernetUdp.h>
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#include <WiFiUdp.h>
#else
#error "Unknown target"
#endif

//! "pool.ntp.org", "time.nist.gov" or IP address
#define NTP_SERVER          "pool.ntp.org"
//! NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE     48
//! UDP listen port
#define NTP_LOCAL_PORT      2390
//! UDP receive timeout
#define NTP_RX_TIMEOUT_MS   1000

/*!
 * \brief NTP client class
 */
class ErriezNTPClient
{
public:
    ErriezNTPClient(const char *ntpServer=NTP_SERVER, uint16_t timeoutMs=NTP_RX_TIMEOUT_MS);

    time_t getEpoch();

private:
#if defined(ARDUINO_ARCH_AVR)
    EthernetUDP _udp;
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
    WiFiUDP     _udp;
#else
#error "Unknown target"
#endif

    bool        _udpInitialized;
    const char *_ntpServer;
    uint32_t    _rxTimeoutMs;

    void sendNTPPacket();
};

#endif // ERRIEZ_NTP_CLIENT_H_