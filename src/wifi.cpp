#include <WiFi.h>
#include "WiFiMulti.h"
#include "sensors.h"
#include "motors.h"
#include "leds.h"
#include "buzzer.h"
#include <ArduinoJson.h>

const char* ssid = "DJ";
const char* password = "Johnson1";

WiFiServer server(80);

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

void sendJsonResponse(WiFiClient& client, const JsonDocument& doc) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println();
    serializeJson(doc, client);
}

void sendRedirectResponse(WiFiClient& client, const char* location) {
    client.println("HTTP/1.1 302 Found");
    client.print("Location: ");
    client.println(location);
    client.println();
}

void handleSensorsRequest(WiFiClient& client) {
    StaticJsonDocument<200> doc;
    
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);
    
    doc["right"] = distance1;
    doc["front"] = distance2;
    doc["back"] = distance3;
    doc["ledMode"] = currentMode;
    doc["buzzerMode"] = currentBuzzerMode;
    
    sendJsonResponse(client, doc);
}

void handleTestRequest(WiFiClient& client) {
    StaticJsonDocument<200> doc;
    doc["status"] = "ok";
    sendJsonResponse(client, doc);
}

void handleClient(WiFiClient& client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            if (c == '\n') {
                if (currentLine.length() == 0) {
                    sendHTMLResponse(client);
                    break;
                } else {
                    if (currentLine.startsWith("GET /sensors")) {
                        handleSensorsRequest(client);
                        break;
                    } else if (currentLine.startsWith("GET /test")) {
                        handleTestRequest(client);
                        break;
                    } else if (currentLine.startsWith("GET /led/off")) {
                        setLEDMode(LED_OFF);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /led/on")) {
                        setLEDMode(LED_ON);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /led/ultrasonic")) {
                        setLEDMode(LED_ULTRASONIC);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /buzzer/off")) {
                        setBuzzerMode(BUZZER_OFF);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /buzzer/on")) {
                        setBuzzerMode(BUZZER_ON_ONESHOT);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /buzzer/ultrasonic")) {
                        setBuzzerMode(BUZZER_ULTRASONIC);
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /S")) {
                        Serial.println("Stop command received");
                        stopMotors();
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /F")) {
                        Serial.println("Forward command received");
                        moveForward();
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /B")) {
                        Serial.println("Backward command received");
                        moveBackward();
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /L")) {
                        Serial.println("Left command received");
                        turnLeft();
                        sendRedirectResponse(client, "/");
                        break;
                    } else if (currentLine.startsWith("GET /R")) {
                        Serial.println("Right command received");
                        turnRight();
                        sendRedirectResponse(client, "/");
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

void sendHTMLResponse(WiFiClient& client) {
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html><head><title>ESP32 Control</title>");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    
    client.println("<style>");
 
    client.println("body { font-family: -apple-system, BlinkMacSystemFont, \"Segoe UI\", Roboto, Oxygen, Ubuntu, Cantarell, \"Open Sans\", \"Helvetica Neue\", sans-serif; background-color: #f0f2f5; color: #333; margin: 0; padding: 15px; }");
    client.println(".container { max-width: 600px; margin: 20px auto; background-color: #ffffff; padding: 25px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }");
    client.println("h1 { text-align: center; color: #1a2b48; margin-bottom: 25px; font-size: 2em; }");
    client.println(".controls-section { margin-bottom: 25px; padding: 20px; background-color: #f8f9fa; border-radius: 6px; border: 1px solid #dee2e6; }");
    client.println("h2 { text-align: center; margin-top: 0; margin-bottom: 20px; color: #495057; border-bottom: 1px solid #e9ecef; padding-bottom: 10px; font-size: 1.5em; }");
    client.println(".row { display: flex; justify-content: center; flex-wrap: wrap; margin-bottom: 10px; }");
    client.println("button { font-size: 1em; padding: 12px 22px; margin: 6px; border-radius: 5px; border: none; cursor: pointer; transition: background-color 0.2s ease, transform 0.1s ease; color: white; font-weight: 500; }");
    client.println("button:hover { filter: brightness(90%); }");
    client.println("button:active { transform: scale(0.98); }");
  
    client.println(".motor-btn { background-color: #6c757d; width: 70px; height: 70px; font-size: 1.8em; padding: 10px; line-height: 1.5em; }"); // Adjusted size for arrows
    client.println(".motor-stop-btn { background-color: #dc3545; width: 90px; font-size: 1em; } "); // Red Stop button, specific width/size
   
    client.println(".led-btn { width: 120px; }");
    client.println(".led-off { background-color: #dc3545; }");
    client.println(".led-on { background-color: #28a745; }");
    client.println(".led-ultrasonic { background-color: #007bff; }");

    client.println(".buzzer-btn { width: 120px; }");
    client.println(".buzzer-off { background-color: #dc3545; }");
    client.println(".buzzer-on { background-color: #ffc107; color: #333; }");
    client.println(".buzzer-ultrasonic { background-color: #007bff; }");
   
    client.println(".sensor { text-align: left; background-color: #e9ecef; padding: 15px; border-radius: 6px; }");
    client.println(".sensor h2 { text-align: left; border: none; padding: 0; margin-bottom: 10px; }");
    client.println(".sensor p { margin: 8px 0; font-size: 1.1em; color: #495057; }");
    client.println(".sensor p span { font-weight: bold; color: #343a40; min-width: 60px; display: inline-block; }");
    
    client.println("@media (max-width: 767px) {");
    client.println("  * { box-sizing: border-box; } /* Include padding and border in element's total width and height */");
    client.println("  body { padding: 0; background-color: #f8f9fa; }");
    client.println("  .app-header { display: block; background-color: #007bff; color: white; padding: 14px 15px; position: sticky; top: 0; z-index: 10; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }"); // Show and style mobile header
    client.println("  .app-header h1 { font-size: 1.15em; margin: 0; text-align: center; font-weight: 600; }"); 
    client.println("  .container { width: 100%; padding: 15px; margin: 0; background-color: #f8f9fa; }"); 
    client.println("  .container > h1 { display: none; } /* Hide desktop H1 specifically on mobile */"); 
    client.println("  h2 { font-size: 0.9em; font-weight: 600; margin-top: 20px; margin-bottom: 10px; padding-bottom: 0; text-align: left; border-bottom: none; color: #6c757d; text-transform: uppercase; letter-spacing: 0.5px; }"); 
    client.println("  .controls-section { padding: 0; margin-bottom: 10px; background-color: transparent; border: none; }"); 
    client.println("  .row { flex-direction: column; align-items: stretch; margin-bottom: 0; }"); 
   
    client.println("  button { display: block; width: 100%; font-size: 1em; padding: 15px; margin-bottom: 8px; border-radius: 8px; border: 1px solid #dee2e6; cursor: pointer; text-align: left; transition: background-color 0.15s ease, border-color 0.15s ease; font-weight: 500; background-color: #ffffff; }"); // White background by default, subtle border
    client.println("  button:active { background-color: #e9ecef; border-color: #adb5bd; transform: none; }"); // Change bg on tap
    client.println("  .led-btn, .buzzer-btn, .motor-btn { width: 100%; max-width: none; height: auto; line-height: normal; }"); // Reset desktop styles

    client.println("  .led-off, .buzzer-off, .motor-stop-btn { background-color: #dc3545; color: white; border-color: #dc3545; }");
    client.println("  .led-on { background-color: #28a745; color: white; border-color: #28a745; }");
    client.println("  .led-ultrasonic, .buzzer-ultrasonic { background-color: #007bff; color: white; border-color: #007bff; }");
    client.println("  .buzzer-on { background-color: #ffc107; color: #333; border-color: #ffc107; }");
    client.println("  .motor-btn { background-color: #ffffff; color: #495057; border-color: #ced4da; } /* Default motor: white bg */"); 
    client.println("  .motor-stop-btn { background-color: #dc3545; color: white; border-color: #dc3545; } /* Stop overrides default */");

    client.println("  .sensor { background-color: #ffffff; padding: 15px; border-radius: 8px; border: 1px solid #dee2e6; margin-top: 20px; }"); // Consistent card style
    client.println("  .sensor h2 { margin-top: 0; }");
    client.println("  .sensor p { font-size: 1em; margin: 8px 0; color: #495057; }");
    client.println("  .sensor p span { font-weight: 600; color: #343a40; }");
    client.println("}");
    
    client.println("footer { text-align: center; margin-top: 30px; padding: 20px 15px; font-size: 0.9em; color: #6c757d; border-top: 1px solid #dee2e6; background-color: #ffffff; }"); // Footer style for desktop
    client.println("footer a { color: #007bff; text-decoration: none; }");
    client.println("footer a:hover { text-decoration: underline; }");
    client.println("footer p { margin: 5px 0; }");
    client.println("@media (max-width: 767px) {");
    client.println("  footer { font-size: 0.8em; padding: 15px 10px; background-color: #f8f9fa; border-top: none; }");
    client.println("}");
    
    client.println("</style>");
    client.println("</head><body>");
    client.println("<div class=\"app-header\"><h1>ULTRANAV</h1></div>");
    client.println("<div class='container'>"); 
    client.println("<h1>ULTRANAV</h1>");
    client.println("<div class='controls-section'>");
    client.println("<h2>Motors</h2>");
    client.println("<div class='row'>"); 
    client.println("<button class='motor-btn' onclick=\"location.href='/F'\" title='Forward'>&uarr;</button>"); 
    client.println("</div>");
    client.println("<div class='row'>");
    client.println("<button class='motor-btn' onclick=\"location.href='/L'\" title='Left'>&larr;</button>");
    client.println("<button class='motor-btn motor-stop-btn' onclick=\"location.href='/S'\">Stop</button>");
    client.println("<button class='motor-btn' onclick=\"location.href='/R'\" title='Right'>&rarr;</button>"); 
    client.println("</div>");
    client.println("<div class='row'>");
    client.println("<button class='motor-btn' onclick=\"location.href='/B'\" title='Backward'>&darr;</button>"); 
    client.println("</div>");
    client.println("</div>");

    client.println("<div class='controls-section'>");
    client.println("<h2>LED Mode</h2>");
    client.println("<div class='row'>");
    client.println("<button class='led-btn led-off' onclick=\"location.href='/led/off'\">OFF</button>");
    client.println("<button class='led-btn led-on' onclick=\"location.href='/led/on'\">ON</button>");
    client.println("<button class='led-btn led-ultrasonic' onclick=\"location.href='/led/ultrasonic'\">ULTRASONIC</button>");
    client.println("</div>");
    client.println("</div>"); 

    client.println("<div class='controls-section'>");
    client.println("<h2>Buzzer Mode</h2>");
    client.println("<div class='row'>");
    client.println("<button class='buzzer-btn buzzer-off' onclick=\"location.href='/buzzer/off'\">OFF</button>");
    client.println("<button class='buzzer-btn buzzer-on' onclick=\"location.href='/buzzer/on'\">ON (1s)</button>");
    client.println("<button class='buzzer-btn buzzer-ultrasonic' onclick=\"location.href='/buzzer/ultrasonic'\">ULTRASONIC</button>");
    client.println("</div>");
    client.println("</div>");

    client.println("<div class='controls-section sensor'>");
    client.println("<h2>Sensor Readings</h2>");
    client.println("<p>Right: <span id=\"dist1-status\">Loading...</span></p>");
    client.println("<p>Front: <span id=\"dist2-status\">Loading...</span></p>");
    client.println("<p>Back: <span id=\"dist3-status\">Loading...</span></p>");
    client.println("</div>"); 

    client.println("</div>");

    client.println("<footer>");
    client.println("  <p>Project: ULTRANAV</p>");
    client.println("  <p>Developer: Denis Johnson (<a href=\"https://github.com/DenisJ99\" target=\"_blank\">DenisJ99</a>)</p>");
    client.println("  <p>Contact: <a href=\"mailto:denisjohnson94@gmail.com\">denisjohnson94@gmail.com</a></p>");
    client.println("  <p>Links: <a href=\"https://github.com/DenisJ99/4thYearProject_RC.Car\" target=\"_blank\">Project Repo</a> | <a href=\"https://www.linkedin.com/in/denis-johnson-b27740278/\" target=\"_blank\">LinkedIn</a></p>");
    client.println("</footer>");

    client.println("<script>");
    client.println("const criticalDist = 5;");
    client.println("const warningDist = 10;");
    client.println("const dist1Element = document.getElementById('dist1-status');");
    client.println("const dist2Element = document.getElementById('dist2-status');");
    client.println("const dist3Element = document.getElementById('dist3-status');");
    
    client.println("function getStatusText(distance) {");
    client.println("  if (distance <= criticalDist) { return 'Very Close'; }");
    client.println("  else if (distance <= warningDist) { return 'Close'; }");
    client.println("  else { return 'Clear'; }");
    client.println("}");
    
    client.println("function updateSensorDisplay(data) {");
    client.println("  if (!data) return;");
    client.println("  dist1Element.textContent = getStatusText(data.right);");
    client.println("  dist2Element.textContent = getStatusText(data.front);");
    client.println("  dist3Element.textContent = getStatusText(data.back);");
    client.println("}");

    client.println("function fetchSensorData() {");
    client.println("  fetch('/sensors')");
    client.println("    .then(response => { if (!response.ok) { throw new Error('Network response was not ok'); } return response.json(); })");
    client.println("    .then(data => { updateSensorDisplay(data); })");
    client.println("    .catch(error => { console.error('Error fetching sensor data:', error); dist1Element.textContent = 'Error'; dist2Element.textContent = 'Error'; dist3Element.textContent = 'Error'; });");
    client.println("}");

    client.println("setInterval(fetchSensorData, 1000);");
    client.println("fetchSensorData();");

    client.println("</script>");
    client.println("</body></html>");
    client.println();
}