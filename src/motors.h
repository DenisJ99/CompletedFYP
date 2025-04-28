#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

#define EN1_PIN 35
#define IN1_PIN 32
#define IN2_PIN 33
#define EN3_PIN 22
#define IN3_PIN 25
#define IN4_PIN 26

enum MovementState {
    STOPPED,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
extern MovementState currentMovement;

void setupMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();

#endif