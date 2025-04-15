#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Ultrasonic sensor pins
extern const int trigPin1;
extern const int echoPin1;
extern const int trigPin2;
extern const int echoPin2;
extern const int trigPin3;
extern const int echoPin3;

// Distance thresholds
extern const int warningDistance;
extern const int criticalDistance;

// Function declarations
void setupSensors();
int getDistance(int trigPin, int echoPin);

#endif