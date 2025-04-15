#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

// Function declarations
void setupWiFi();
void handleClient(WiFiClient& client);
void sendHTMLResponse(WiFiClient& client);

// Server declaration
extern WiFiServer server;

#endif 