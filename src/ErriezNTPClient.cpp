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
 * \file ErriezNTPClient.cpp
 * \brief NTP client library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezNTPClient
 *      Documentation:  https://erriez.github.io/ErriezNTPClient
 */

#include "ErriezNTPClient.h"


/*!
 * \brief Constructor
 * \param ntpServer
 *      NTP server
 * \param timeoutMs
 *      UDP receive timeout in ms
 */
ErriezNTPClient::ErriezNTPClient(const char *ntpServer, uint16_t timeoutMs) :
    _udpInitialized(false), _ntpServer(ntpServer), _rxTimeoutMs(timeoutMs)
{
}

/*!
 * \brief Send NTP UDP Ethernet packet
 */
void ErriezNTPClient::sendNTPPacket()
{
    uint8_t txBuffer[NTP_PACKET_SIZE] = { 0 };

    // Initialize values needed to form NTP request
    txBuffer[0] = 0b11100011; // LI, Version, Mode
    txBuffer[1] = 0;          // Stratum, or type of clock
    txBuffer[2] = 6;          // Polling Interval
    txBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    txBuffer[12]  = 49;
    txBuffer[13]  = 0x4E;
    txBuffer[14]  = 49;
    txBuffer[15]  = 52;

    // Send NTP packet to port 123
    _udp.beginPacket(_ntpServer, 123);
    _udp.write(txBuffer, NTP_PACKET_SIZE);
    _udp.endPacket();
}

/*!
 * \brief Get UNIX Epoch UTC time
 * \return
 *      UNIX Epoch in UTC
 */
time_t ErriezNTPClient::getEpoch()
{
    uint8_t rxBuffer[NTP_PACKET_SIZE];
    uint32_t _tPacketSent;
    time_t t = 0;

    // Initialize UDP
    if (!_udpInitialized) {
        _udp.begin(NTP_LOCAL_PORT);
        _udpInitialized = true;
    }

    // Send packet
    sendNTPPacket();

    // Store timestamp send packet
    _tPacketSent = millis();

    // Receive UDP packet with receive timeout
    do {
        // Check if packet received
        if (_udp.parsePacket()) {
            // Read data from received UDP packet
            _udp.read(rxBuffer, NTP_PACKET_SIZE);

            // NTP time in seconds since Jan 1 1900
            unsigned long highWord = word(rxBuffer[40], rxBuffer[41]);
            unsigned long lowWord = word(rxBuffer[42], rxBuffer[43]);

            t = (highWord << 16) | lowWord;

            // Unix time starts on Jan 1 1970. In seconds, that's 2208988800
            t -= 2208988800UL;
            break;
        }
    } while ((millis() - _tPacketSent) < _rxTimeoutMs);

    // Return epoch
    return t;
}