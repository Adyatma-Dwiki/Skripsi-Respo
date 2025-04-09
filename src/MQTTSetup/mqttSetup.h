#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "TFTSetup/TFTSetup.h" // Jika perlu akses TFT

#define BUTTON_PIN 25 // Sesuaikan pin tombol

extern WiFiClient espClient;
extern PubSubClient client;

extern bool showMessage;
extern unsigned long timeHiddenUntil;


void callback(char *topic, byte *payload, unsigned int length);
void setupMQTT();
void reconnectMQTT();
void sendStatus(const char *status);
void handleButton();

#endif
