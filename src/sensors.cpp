#include "sensors.h"

const int trigPin1 = 5;
const int echoPin1 = 18;
const int trigPin2 = 12;
const int echoPin2 = 13;
const int trigPin3 = 14;
const int echoPin3 = 27;

const int warningDistance = 10;
const int criticalDistance = 5;

void setupSensors() {
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
}

int getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}