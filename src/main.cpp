#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include "wifiSetup/wifiSetup.h"
#include "MQTTSetup/mqttSetup.h"
#include "TFTSetup/TFTSetup.h"  // Tambahkan header TFT

#define BL 4
#define IO2 2

const char* ntpServer = "pool.ntp.org";
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
    client.loop();
    static unsigned long lastTimeUpdate = 0;

    if (millis() - lastTimeUpdate > 1000) {
        lastTimeUpdate = millis();
        displayTime();
    }

    if (!client.connected()) {
        reconnectMQTT();
    }

    delay(100);
}
