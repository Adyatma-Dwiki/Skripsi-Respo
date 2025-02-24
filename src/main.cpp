#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include "wifiSetup/wifiSetup.h"
#include "MQTTSetup/mqttSetup.h"
#include "TFTSetup/TFTSetup.h"  // Tambahkan header TFT

#define BL 4
#define IO2 2

const char* ntpServer = "pool*ntp*org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Memulai Setup...");

    pinMode(BL, OUTPUT);
    pinMode(IO2, OUTPUT);
    analogWrite(BL, 100);
    digitalWrite(IO2, HIGH);

    initTFT();  // Panggil fungsi inisialisasi TFT

    // Koneksi ke WiFi
    connectWiFi();

    // Sinkronisasi waktu dengan NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);

    // Koneksi ke MQTT
    setupMQTT();
    reconnectMQTT();

    displayTime();  // Tampilkan jam pertama kali
}

void loop() {
    client.loop(); // Proses MQTT

    static unsigned long lastTimeUpdate = 0;

    // Jika tidak ada pesan aktif atau sudah lewat 2 menit sejak terakhir pesan
    if (!showMessage || millis() > timeHiddenUntil) {  
        if (millis() - lastTimeUpdate > 1000) {  // Update jam setiap 1 detik
            lastTimeUpdate = millis();
            
            // **Bersihkan layar hanya saat pertama kali kembali ke mode jam**
            if (showMessage) {
                tft.fillScreen(TFT_BLACK); // Hapus sisa tampilan pesan
                showMessage = false;       // Reset status pesan
            }
            
            // displayTime();
        }
    }

    delay(100);
}


