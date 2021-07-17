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

int cps;  // counts per second 
MovingAverage<unsigned long> cp_buffer(PERIOD_SIZE);  // buffer to store values during the measured period

TinyTimer<millis> timers[4];

#include "secret.h"
#include "display.h"
#include "radmon.h"
#include "mqtt.h"

// simply increment the counter on every interrupt
static void addSample() { cps++; }

// add number of iterrupts to buffer every second
static void storeToBuffer()
{
    noInterrupts();
    cp_buffer.push(cps);
    cps = 0;
    interrupts();
}

void logToCloud()
{
    int ppm = cp_buffer.getAverage() /* [s] */ * 60.0;

    logToMqtt(ppm);
    logToRadmon(ppm);
}

void setup()
{
    Serial.begin(SERIAL_SPEED);
    pinMode(GEIGER_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(GEIGER_PIN), addSample, FALLING);

    // initialize timers
    timers[0].init(storeToBuffer, 1000);  // store reading into buffer
    timers[1].init(showMeasurements, PRINT_INTERVAL);  // show measured values
    timers[2].init(updateBitmap, 200);  // refresh the animation
    timers[3].init(logToCloud, LOG_INTERVAL);  // log to MQTT and Radmon.org

    // initialize display
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x32

    display.clearDisplay();

    // connect to Wifi
    Serial.print("Connecting to ");
    Serial.println(SSID);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Connecting to");
    display.setCursor(0, 10);
    display.print(SSID);
    display.display();

    WiFi.begin(SSID, WIFI_PASSWORD);

    int xpos = 0, i=0;
    display.setTextSize(1);
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
    display.display();

    delay(1000);

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(110, 24);
    display.print("cpm");

    display.setTextSize(1);
    display.setCursor(2 * 4 * 6 + 4, 2);
    display.print("cpm");

    display.setCursor(2 * 4 * 6 + 4, 18);
    display.print("uSi/h");

    updateBitmap();
}

void loop()
{
    TinyTimer<millis>::update(timers);
}
