#ifndef MQTTSETUP_H
#define MQTTSETUP_H

#include <PubSubClient.h>

extern PubSubClient client;  // Variabel global agar bisa digunakan di main.cpp
extern bool showMessage;
extern unsigned long timeHiddenUntil;


void setupMQTT();
void reconnectMQTT();

#endif
