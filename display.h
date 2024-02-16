/*
 * Function related to display
 */

#define TEXT_SIZE_1 1
#define TEXT_SIZE_2 4

Adafruit_SSD1306 display(128, 64, &Wire);
#define SCREEN_ADDRESS 0x3C  // Address 0x3C for 128x32, 0x3D for 128x64

// we do not want to clear whole display, only invert last shown value (much faster)
String lastShownValue1, lastShownValue2;

void clearPrevValues() {
    display.setTextSize(TEXT_SIZE_2);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 0);
    display.print(lastShownValue1);
    display.setCursor(0, TEXT_SIZE_2 * 8);
    display.print(lastShownValue2);
}

void displayInit()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // initialize display
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // Address 0x3C for 128x32, 0x3D for 128x64

    //display.display();
    //display.clearDisplay();

}

// called at the end of the set time period (PERIOD_SIZE)
static void showMeasurements()
{
    char buf[10];
    int ppm = round(cp_buffer.getAverage() /* [s] */ * 60.0);
    Serial.print("      ");
    Serial.println(ppm);
    float uSvh;  //variable for uSv/h

    Serial.println(ppm);

    clearPrevValues();
    // text display tests
    display.setTextSize(TEXT_SIZE_2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    sprintf(buf, "%4d", ppm);
    lastShownValue1 = String(buf);
    display.print(lastShownValue1);

    display.setCursor(0, TEXT_SIZE_2 * 8);
    // microsieverts per hour = (impulses per minute) * 0.0092
    // tubes may vary (+-20%), so we recommend using a conversion factor from 0.0054 to 0.0092 and calibrate your calculations against a trusted (certified) device.
    uSvh = cp_buffer.getAverage() /* [s] */ * 60.0 * 0.0092;//0073;
    sprintf(buf, "%.2f", uSvh);
    lastShownValue2 = String(buf);
    display.print(lastShownValue2);

    display.display();
}


int bitmapIdx = 0;
unsigned long lastCp = 0;
static void updateBitmap()
{
    if (cp == lastCp) return;  // no need to update the animation if there was no interrupt
    lastCp = cp;

  //  interrupts();
    display.drawBitmap(128 - 24, 18, epd_bitmap_allArray[bitmapIdx], 24, 24, BLACK);

    bitmapIdx++;
    if (bitmapIdx == epd_bitmap_allArray_LEN) bitmapIdx = 0;

    display.drawBitmap(128 - 24, 18, epd_bitmap_allArray[bitmapIdx], 24, 24, WHITE);

    display.display();
  //  interrupts();
}
