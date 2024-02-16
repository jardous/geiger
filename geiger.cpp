#include <Arduino.h>
#include <limits.h>

#include <WiFi.h>

// display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "libs/MovingAverage.h"
#include "libs/TinyTimer.h"

#include "bitmaps.h"


#define SERIAL_SPEED     115200
#define GEIGER_PIN       A5

#define PERIOD_SIZE      30
#define PRINT_INTERVAL   1000
#define LOG_INTERVAL     10000

#define OFFLINE_MODE

unsigned long cps;  // counts per second
unsigned long cp;  // overall pulse counts
MovingAverage<unsigned long> cp_buffer(PERIOD_SIZE);  // buffer to store values during the measured period

TinyTimer<millis> timers[3];

#include "secret.h"
#include "display.h"
#include "radmon.h"
#include "mqtt.h"

// simply increment the counter on every interrupt
static void addSample() { cps++; cp++; Serial.println("."); }

// add number of iterrupts to buffer every second
static void storeToBuffer()
{
    Serial.println(cps);
    //noInterrupts();
    cp_buffer.push(cps);
    cps = 0;
    //interrupts();
}

void logToCloud()
{
#ifndef OFFLINE_MODE
    int ppm = cp_buffer.getAverage() /* [s] */ * 60.0;

    logToMqtt(ppm);
    //logToRadmon(ppm);
#endif
}


void setup()
{
    Serial.begin(SERIAL_SPEED);
    pinMode(GEIGER_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(GEIGER_PIN), addSample, FALLING);

    // initialize timers
    timers[0].init(storeToBuffer, 1000);  // store reading into buffer
    timers[1].init(showMeasurements, PRINT_INTERVAL);  // show measured values
    timers[2].init(updateBitmap, 200);  // refresh the animation
#ifndef OFFLINE_MODE
    timers[3].init(logToCloud, LOG_INTERVAL);  // log to MQTT and Radmon.org
#endif

    displayInit();

#ifndef OFFLINE_MODE
    // connect to Wifi
    Serial.print("Connecting to ");
    Serial.println(SSID);

    display.setTextSize(TEXT_SIZE_1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Connecting to");
    display.setCursor(0, 10);
    display.print(SSID);
    display.display();

    WiFi.begin(SSID, WIFI_PASSWORD);

    int xpos = 0, i=0;
    display.setTextSize(TEXT_SIZE_1);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        display.setCursor(xpos*i, 24);
        i++;

        if (i == 10) {
          break;
        }
        
        display.print(".");
        display.display();
    }

    if (i == 10) {
        display.print("timeout");
    } else {
        display.print("connected");
    }
#endif
    display.display();

    delay(1000);

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    lastCp = -1;
/*
    display.setTextSize(TEXT_SIZE_1);
    display.setCursor(110, 24);
    display.print("cpm");
*/
    display.setTextSize(TEXT_SIZE_1);
    display.setCursor(TEXT_SIZE_2 * 4 * 6 + 2, 0);
    display.print("cpm");

    display.setCursor(TEXT_SIZE_2 * 4 * 6 + 2, 64-12);
    display.print("uSi/h");

    updateBitmap();
}

void loop()
{
    TinyTimer<millis>::update(timers);
}
