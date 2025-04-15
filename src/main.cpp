#include <Arduino.h>
#include "leds.h"
#include "buzzer.h"
#include "motors.h"
#include "sensors.h"
#include "wifi.h"

void setup() {
    Serial.begin(115200);
    setupLED();
    setupBuzzer();
    setupMotors();
    setupSensors();
    setupWiFi();
}

void loop() {
    WiFiClient client = server.available();
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    bool objectDetected = (distance1 <= warningDistance || distance2 <= warningDistance || distance3 <= warningDistance);
    bool objectClose = (distance1 <= criticalDistance || distance2 <= criticalDistance || distance3 <= criticalDistance);

    if (objectClose) {
        alertLED(true);
        alertBuzzer(true);
    } else if (objectDetected) {
        alertLED(false);
        alertBuzzer(false);
    } else {
        digitalWrite(Led, LOW);
        ledcWrite(buzzerChannel, 0);
    }

    if (client) {
        handleClient(client);
    }
}