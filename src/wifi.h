#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

void setupWiFi();
void handleClient(WiFiClient& client);
void sendHTMLResponse(WiFiClient& client);

extern WiFiServer server;

#endif 