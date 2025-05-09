#include "mqttSetup.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft; // Gunakan TFT dari main.cpp
WiFiClient espClient;  // Definisi objek WiFiClient
PubSubClient client(espClient);  // Inisialisasi client MQTT
const char *mqttServer = "192.168.0.100";
const int mqttPort = 1884;
String currentOrder = "";  // Menyimpan order saat ini
int currentTable = 0;      // Menyimpan nomor meja aktif
int currentOrderID = 0;
unsigned long buttonPressStart = 0;
bool helloSent = false;

unsigned long messageStartTime = 0;
bool buttonPressed = false;  
bool orderTaken = false; 
bool hideTime = false;  // Status apakah waktu disembunyikan atau tidak


// Callback ketika ada pesan baru dari MQTT
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Pesan diterima dari topik: ");
    Serial.println(topic);

    hideTime = true;  // Sembunyikan waktu

    char receivedMessage[256] = "";
    memset(receivedMessage, 0, sizeof(receivedMessage));
    for (int i = 0; i < length && i < sizeof(receivedMessage) - 1; i++) {
        receivedMessage[i] = (char)payload[i];
    }

    Serial.print("Isi pesan: ");
    Serial.println(receivedMessage);

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, receivedMessage);
    if (error) {
        Serial.print("JSON Parsing gagal: ");
        Serial.println(error.f_str());
        return;
    }

    const int table = doc["table_id"];
    const char *order = doc["id"];
    const char *status = doc["status"];
    currentTable = table;  // Simpan nomor meja saat ini
    currentOrderID = doc["id"].as<int>(); // Simpan order saat ini

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

void buttonSendHelloMessage() {
    static bool button2Pressed = false;

    if (digitalRead(BUTTON_PIN2) == HIGH && !button2Pressed) {
        button2Pressed = true;

        // Kirim hello langsung
        Serial.println("Kirim hello!");
        sendHelloMessage();

        while (digitalRead(BUTTON_PIN2) == HIGH) { delay(50); }  // Tunggu tombol dilepas
        delay(300);  // Debounce
    }

    if (digitalRead(BUTTON_PIN2) == LOW) {
        button2Pressed = false;
    }
}



void sendHelloMessage() {
    StaticJsonDocument<128> doc;
    doc["message"] = "hello";
    doc["from"] = "jam_tangan";

    char buffer[128];
    serializeJson(doc, buffer);

    client.publish("dapur/hello", buffer);
}


void sendStatus(const char *status)
{
    StaticJsonDocument<256> doc;
    doc["status"] = status;
    doc["table_id"] = currentTable;
    doc["id"] = currentOrderID;
    


    char buffer[256];
    serializeJson(doc, buffer);

    client.publish("dapur/response", buffer);
    Serial.println("Pesan terkirim: " + String(buffer));
}

// **Tombol untuk Mengubah Status**
void handleButton() {
    static bool waitingForSecondPress = false;  // Menunggu tekan kedua

    if (digitalRead(BUTTON_PIN) == HIGH && !buttonPressed) {  // Tombol pertama kali ditekan
        buttonPressed = true;

        if (!waitingForSecondPress) {  
            // **TEKANAN PERTAMA**: Pegawai mengambil pesanan
            Serial.println("Pegawai mengambil pesanan!");
            sendStatus("Pegawai mengambil");

            // Masuk ke mode menunggu tekan kedua
            waitingForSecondPress = true;
        } else {
            // **TEKANAN KEDUA**: Pegawai selesai mengantar
            Serial.println("Pegawai selesai mengantar pesanan!");
            sendStatus("Pegawai selesai mengantar");

            // Reset order agar bisa menerima order baru
            orderTaken = false;
            currentOrder = "";
            currentTable = 0;
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(30, 40);
            tft.println("Menunggu order...");

            hideTime = false;  // Tampilkan kembali waktu setelah selesai

            // Reset agar bisa kembali ke tekan pertama
            waitingForSecondPress = false;
        }

        while (digitalRead(BUTTON_PIN) == HIGH) { delay(50); }  // Tunggu tombol dilepas
        delay(300);  // Debounce
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
        buttonPressed = false;  // Reset tombol agar bisa ditekan lagi
    }
}
