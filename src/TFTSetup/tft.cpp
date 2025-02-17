#include "TFTSetup.h"
#include <time.h>  // Untuk mendapatkan waktu dari NTP

TFT_eSPI tft = TFT_eSPI();

String getTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "00:00:00";
    }
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    return String(timeStr);
}

void initTFT() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(50, 50);
    tft.println("Booting...");
}

void displayTime() {
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(50, 50);
    tft.fillRect(50, 10, 100, 20, TFT_BLACK); // Bersihkan area sebelumnya
    tft.println(getTime());
}
