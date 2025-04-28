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

    if (currentMovement == BACKWARD && (distance1 <= criticalDistance || distance2 <= criticalDistance || distance3 <= criticalDistance)) {
        stopMotors();
        Serial.println("Auto-stop: Obstacle very close while reversing!");
    }

    updateLEDs(distance1, distance2, distance3);
    updateBuzzer(distance1, distance2, distance3);

    if (client) {
        handleClient(client);
    }
}