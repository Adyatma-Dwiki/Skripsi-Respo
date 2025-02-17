#ifndef MQTTSETUP_H
#define MQTTSETUP_H

#include <PubSubClient.h>

extern PubSubClient client;  // Variabel global agar bisa digunakan di main.cpp

void setupMQTT();
void reconnectMQTT();

#endif
