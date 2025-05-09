#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include "wifiSetup/wifiSetup.h"
#include "MQTTSetup/mqttSetup.h"
#include "TFTSetup/TFTSetup.h"

#define BL 4
#define IO2 2
#define BUTTON_PIN 25
#define BUTTON_PIN2 35

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;
unsigned long timeHiddenUntil = 0;
bool showMessage = false;
extern bool hideTime;

unsigned long lastTimeUpdate = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Memulai Setup...");

    pinMode(BL, OUTPUT);
    pinMode(IO2, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN2, INPUT_PULLUP);

    // Konfigurasi LED Backlight
    ledcAttachPin(BL, 1);
    ledcSetup(1, 5000, 8);
    ledcWrite(1, 100);

    digitalWrite(IO2, HIGH);

    initTFT();
    connectWiFi();

    // Konfigurasi waktu NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);

    setupMQTT();
}

void loop() {
    if (!hideTime) {  // Jika tidak ada pesan MQTT, tampilkan waktu
        displayTime();
    }

    if (!client.connected()) {
        reconnectMQTT();

    }
    client.loop();  // Pastikan MQTT tetap berjalan

    buttonSendHelloMessage();  // Cek tombol untuk mengirim pesan "Hello"
    handleButton();  // Cek tombol

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi terputus! Menghubungkan ulang...");
        connectWiFi();
    }

    delay(100);
}

