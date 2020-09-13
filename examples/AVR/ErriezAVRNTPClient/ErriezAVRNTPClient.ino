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
 * \brief Erriez ESP8266 NTP UTC example
 * \details
 *      Source:         https://github.com/Erriez/ErriezArduinoExamples
 */

#include <Ethernet.h>
#include <ErriezNTPClient.h>

// "pool.ntp.org", "time.nist.gov" or NTP server IP address
#define NTP_SERVER          "pool.ntp.org"

ErriezNTPClient ntp(NTP_SERVER);

// Newer Ethernet shields have a MAC address printed on a sticker on the shield
uint8_t mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};


void setup()
{
    // Initialize serial
    delay(500);
    Serial.begin(115200);
    Serial.println(F("\nErriez NTP client AVR example"));

    // Start Ethernet and UDP
    if (!Ethernet.begin(mac)) {
        Serial.println(F("Failed to configure Ethernet using DHCP"));

        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println(F("Ethernet shield was not found."));
        } else if (Ethernet.linkStatus() == LinkOFF) {
            Serial.println(F("Ethernet cable is not connected."));
        }
    }
}

void loop()
{
    time_t t;

    // Get epoch
    t = ntp.getEpoch();

    // Print result
    if (t > 0) {
        Serial.print(F("Epoch: "));
        Serial.println((uint32_t)t);

        // A UNIX offset is needed for AVR target
        t -= UNIX_OFFSET;

        Serial.print(F("UTC:   "));
        Serial.println(ctime(&t));
    } else {
        Serial.println(F("Timeout"));
    }

    delay(10000);
}