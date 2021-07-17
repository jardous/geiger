/*
 * Function related to display
 */

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

// we do not want to clear whole display, only invert last shown value (much faster)
String lastShownValue1, lastShownValue2;

void clearPrevValues() {
    display.setTextSize(2);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 0);
    display.print(lastShownValue1);
    display.setCursor(0, 2 * 8);
    display.print(lastShownValue2);
}

// called at the end of the set time period (PERIOD_SIZE)
static void showMeasurements()
{
    char buf[10];
    int ppm = round(cp_buffer.getAverage() /* [s] */ * 60.0);
    float uSvh;  //variable for uSv/h

    Serial.println(ppm);

    clearPrevValues();
    // text display tests
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    sprintf(buf, "%4d", ppm);
    lastShownValue1 = String(buf);
    display.print(lastShownValue1);

    display.setCursor(0, 2 * 8);
    // microsieverts per hour = (impulses per minute) * 0.0092
    // tubes may vary (+-20%), so we recommend using a conversion factor from 0.0054 to 0.0092 and calibrate your calculations against a trusted (certified) device.
    uSvh = cp_buffer.getAverage() /* [s] */ * 60.0 * 0.0092;//0073;
    sprintf(buf, "%.2f", uSvh);
    lastShownValue2 = String(buf);
    display.print(lastShownValue2);

    display.display();
}


int bitmapIdx = 0;
int lastCps = 0;
static void updateBitmap()
{
    if (cps == lastCps) return;  // no need to update the animation if there was no interrupt
    lastCps = cps;
  //  interrupts();
    display.drawBitmap(128 - 24, 0, epd_bitmap_allArray[bitmapIdx], 24, 24, BLACK);

    bitmapIdx++;
    if (bitmapIdx == epd_bitmap_allArray_LEN) bitmapIdx = 0;

    display.drawBitmap(128 - 24, 0, epd_bitmap_allArray[bitmapIdx], 24, 24, WHITE);

    display.display();
  //  interrupts();
}
