#include "TFT_eSPI.h"
#include <time.h>  // Untuk mendapatkan waktu dari NTP

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);  // Buat sprite untuk double buffering

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
    String msg = "Booting...";
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    // Hitung posisi agar teks di tengah
    int textWidth = tft.textWidth(msg);
    int textHeight = tft.fontHeight();
    int xx = (240 - textWidth) / 2;
    int yy = (240 - textHeight) / 2;

    // Tampilkan teks booting
    tft.setCursor(xx, yy);
    tft.println(msg);

    delay(2000);  // Tampilkan "Booting..." selama 2 detik
    tft.fillScreen(TFT_BLACK);  // Hapus teks booting dengan membersihkan layar

    // Inisialisasi sprite
    sprite.setColorDepth(8);
    sprite.createSprite(240, 240);  // Ukuran layar TFT
    sprite.fillSprite(TFT_BLACK);   // Isi sprite dengan warna hitam
}

void displayTime() {
    sprite.fillSprite(TFT_BLACK);  // Bersihkan sprite tanpa flickering
    sprite.setTextSize(3);
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);

    String msg = getTime();

    // Hitung ulang posisi agar selalu di tengah
    int textWidth = sprite.textWidth(msg);
    int textHeight = sprite.fontHeight();
    int xx = (240 - textWidth) / 2;
    int yy = (240 - textHeight) / 2;

    // Gambar teks di sprite, bukan langsung di layar
    sprite.setCursor(xx, yy);
    sprite.println(msg);

    // Tampilkan sprite ke layar sekaligus (mengurangi flickering)
    sprite.pushSprite(0, 0);
}
