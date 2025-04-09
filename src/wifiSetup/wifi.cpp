#include "wifi.h"
#include <Arduino.h>

const char* ssid = "Kamar";
const char* password = "kamardepan";

void connectWiFi() {
    Serial.print("Menghubungkan ke WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    int attempts = 0;
    
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Terhubung!");
        Serial.print("IP ESP32: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());
    } else {
        Serial.println("\nGagal terhubung ke WiFi!");
    }
}
