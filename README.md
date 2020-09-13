# Erriez NTP Client library for Arduino
[![Build Status](https://travis-ci.org/Erriez/ErriezNTPClient.svg?branch=master)](https://travis-ci.org/Erriez/ErriezNTPClient)

This is a minimized NTP Client library for Arduino to retrieve UNIX Epoch UTC from NTP time servers.


## Library features

* Retrieve UNIX Epoch UTC time from network time servers
* Compatible with `time.h`
* Timeout handling


## Supported hardware

* Arduino UNO with EtherShield (Wiznet W5100 Ethernet controller)
* ESP8266 WiFi
* ESP32 WiFi


## Documentation

- [Online HTML](https://erriez.github.io/ErriezNTPClient)
- [Doxygen PDF](https://github.com/Erriez/ErriezNTPClient/raw/master/ErriezNTPClient.pdf)


## Example output

```c++
Erriez ESP8266 NTP example
Connecting to 'wifi'...OK
Epoch: 1600025290
UTC:   Sun Sep 13 19:28:10 2020
```


## Example ESP8266 / ESP32

```c++
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

#include <ErriezNTPClient.h>

// WiFi SSID and Password
#define WIFI_SSID           ""
#define WIFI_PASSWORD       ""

// "pool.ntp.org", "time.nist.gov" or NTP server IP address
#define NTP_SERVER          "pool.ntp.org"

ErriezNTPClient ntp(NTP_SERVER);


void setup()
{
    // Initialize serial
    delay(500);
    Serial.begin(115200);
    Serial.println(F("\nErriez NTP client ESP8266 / ESP32 example"));

    // Initialize WiFi
    Serial.print(F("Connecting to '"));
    Serial.print(WIFI_SSID);
    Serial.print(F("'"));

    // Connect to your WiFi router
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("OK");
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
        Serial.print(F("UTC:   "));
        Serial.println(ctime(&t));
    } else {
        Serial.println(F("Timeout"));
    }

    delay(10000);
}
```


## Example AVR (ATMega328 / ATMega2560)

```c++
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
```


## Library installation

Please refer to the [Wiki](https://github.com/Erriez/ErriezArduinoLibraries/wiki) page.


## Other Arduino Libraries and examples from Erriez

[Erriez Libraries](https://github.com/Erriez/ErriezArduinoLibraries)

