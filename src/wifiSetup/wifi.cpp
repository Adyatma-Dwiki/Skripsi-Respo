#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiManager.h>

void connectWiFi() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n🚀 Inisialisasi WiFi Manager...");

    WiFiManager wm;

    // Coba konek ke WiFi yang sudah disimpan sebelumnya
    if (WiFi.SSID() != "" && WiFi.psk() != "") {
        Serial.println("🔁 Mencoba konek ke WiFi tersimpan...");
        WiFi.begin(); // konek ke WiFi terakhir
        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 20) {
            delay(500);
            Serial.print(".");
            retry++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ Berhasil terhubung ke WiFi yang tersimpan!");
            Serial.print("IP ESP32: ");
            Serial.println(WiFi.localIP());
            return;
        }
        Serial.println("\n⚠️ Gagal konek ke WiFi tersimpan!");
    }

    // Kalau gagal konek, buka config portal
    Serial.println("📶 Membuka WiFi Config Portal...");
    if (!wm.startConfigPortal("RestoESP32", "resto1234")) {
        Serial.println("❌ Konfigurasi WiFi dibatalkan. Restart...");
        delay(3000);
        ESP.restart();
    }

    // Setelah konek
    Serial.println("✅ WiFi tersambung melalui portal!");
    Serial.print("IP ESP32: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

#endif
