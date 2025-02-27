#include "mqttSetup.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft; // Gunakan TFT dari main.cpp

const char *mqttServer = "192.168.0.100";
const int mqttPort = 1884;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long messageStartTime = 0;
bool showMessage = false;
unsigned long timeHiddenUntil = 0;

// Callback ketika ada pesan baru dari MQTT
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Pesan diterima dari topik: ");
    Serial.println(topic);

    char receivedMessage[256] = "";
    memset(receivedMessage, 0, sizeof(receivedMessage));
    for (int i = 0; i < length && i < sizeof(receivedMessage) - 1; i++)
    {
        receivedMessage[i] = (char)payload[i];
    }

    Serial.print("Isi pesan: ");
    Serial.println(receivedMessage);

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, receivedMessage);
    if (error)
    {

        Serial.print("JSON Parsing gagal: ");
        Serial.println(error.f_str());
        return;
    }

    const int table = doc["table_id"];
    const char *status = doc["status"];
    int xx, yy;
    String msg = "Pesanan Baru";
    int x = tft.fontHeight();
    int y = tft.textWidth(msg);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    xx = ((240 / 2) - (x / 2))-20;
    yy = ((240 / 2) - (y / 2));

    tft.setCursor(yy, xx);
    tft.println(msg);

    int newx = xx + 20;
    tft.setCursor(yy, newx);
    tft.setTextSize(2);
    tft.print("Table: ");
    tft.println(table);

    // show data makanan
    //  tft.setTextSize(1);
    //  JsonArray foodArray = doc["food_names"].as<JsonArray>();
    //  int yPos = 90;
    //  for (const char* foodName : foodArray) {
    //      tft.setCursor(20, yPos);
    //      tft.println(foodName);
    //      yPos += 20;
    //  }

    // Show status data
    newx += 20;
    tft.setTextSize(2);
    tft.setCursor(yy, newx);
    tft.println(status);

    messageStartTime = millis();
    showMessage = true;

    // **Set waktu hingga displayTime() harus dihentikan (2 menit dari sekarang)**
    timeHiddenUntil = millis() + 120000;
}

// Fungsi untuk menghubungkan ke MQTT
void setupMQTT()
{
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
}

// Fungsi untuk memastikan koneksi tetap berjalan
void reconnectMQTT()
{
    while (!client.connected())
    {
        Serial.println("Menghubungkan ke MQTT...");
        if (client.connect("ESP32Client"))
        {
            Serial.println("Terhubung ke MQTT!");
            client.subscribe("dapur/order");
        }
        else
        {
            Serial.print("Gagal, kode error: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}
