#include <WiFi.h>
#include "WiFiMulti.h"
#include "sensors.h"
#include "motors.h"
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "DJ";
const char* password = "Johnson1";

// Server setup
WiFiServer server(80);

// Function to connect to WiFi
void setupWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected.");
    Serial.println(WiFi.localIP());
    server.begin();
}

// Function to send JSON response
void sendJsonResponse(WiFiClient& client, const JsonDocument& doc) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println();
    serializeJson(doc, client);
}

// Function to handle sensor data request
void handleSensorsRequest(WiFiClient& client) {
    StaticJsonDocument<200> doc;
    
    doc["right"] = getDistance(trigPin1, echoPin1);
    doc["front"] = getDistance(trigPin2, echoPin2);
    doc["back"] = getDistance(trigPin3, echoPin3);
    
    sendJsonResponse(client, doc);
}

// Function to handle test connection request
void handleTestRequest(WiFiClient& client) {
    StaticJsonDocument<200> doc;
    doc["status"] = "ok";
    sendJsonResponse(client, doc);
}

// Function to handle client requests
void handleClient(WiFiClient& client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            if (c == '\n') {
                if (currentLine.length() == 0) {
                    // Send the default HTML response
                    sendHTMLResponse(client);
                    break;
                } else {
                    // Handle API requests
                    if (currentLine.startsWith("GET /sensors")) {
                        handleSensorsRequest(client);
                        break;
                    } else if (currentLine.startsWith("GET /test")) {
                        handleTestRequest(client);
                        break;
                    } else if (currentLine.endsWith("GET /S")) {
                        stopMotors();
                        sendJsonResponse(client, StaticJsonDocument<200>());
                        break;
                    } else if (currentLine.endsWith("GET /F")) {
                        moveForward();
                        sendJsonResponse(client, StaticJsonDocument<200>());
                        break;
                    } else if (currentLine.endsWith("GET /B")) {
                        moveBackward();
                        sendJsonResponse(client, StaticJsonDocument<200>());
                        break;
                    } else if (currentLine.endsWith("GET /L")) {
                        turnLeft();
                        sendJsonResponse(client, StaticJsonDocument<200>());
                        break;
                    } else if (currentLine.endsWith("GET /R")) {
                        turnRight();
                        sendJsonResponse(client, StaticJsonDocument<200>());
                        break;
                    }
                    currentLine = "";
                }
            } else if (c != '\r') {
                currentLine += c;
            }
        }
    }
    client.stop();
    Serial.println("Client Disconnected.");
}

// Function to send HTML response to the client
void sendHTMLResponse(WiFiClient& client) {
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>ESP32 Motor Control</h1>");
    client.println("<style>");
    client.println("button { font-size: 24px; padding: 10px 20px; margin: 5px; }");
    client.println(".arrow { font-size: 36px; width: 60px; height: 60px; }");
    client.println(".container { display: flex; flex-direction: column; align-items: center; }");
    client.println(".row { display: flex; justify-content: center; }");
    client.println(".sensor { margin-top: 20px; font-size: 18px; }");
    client.println("</style>");
    client.println("<div class='container'>");
    client.println("<div class='row'>");
    client.println("<button onclick=\"location.href='/F'\">Go</button>");
    client.println("</div>");
    client.println("<div class='row'>");
    client.println("<button onclick=\"location.href='/L'\">Left</button>");
    client.println("<button onclick=\"location.href='/S'\">Stop</button>");
    client.println("<button onclick=\"location.href='/R'\">Right</button>");
    client.println("</div>");
    client.println("<div class='row'>");
    client.println("<button onclick=\"location.href='/B'\">Back</button>");
    client.println("</div>");
    client.println("<div class='sensor'>");
    client.println("<p>Right Ultrasonic Distance: " + String(distance1) + " cm</p>");
    client.println("<p>Ultrasonic Sensor 2 Distance: " + String(distance2) + " cm</p>");
    client.println("<p>Back Ultrasonic Distance: " + String(distance3) + " cm</p>");
    client.println("</div>");
    client.println("</div>");
    client.println("</body></html>");
    client.println();
}