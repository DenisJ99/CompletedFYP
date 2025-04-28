#include "motors.h"

MovementState currentMovement = STOPPED;

void setupMotors() {
    pinMode(EN1_PIN, OUTPUT);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    stopMotors();
}

void moveForward() {
    currentMovement = FORWARD;
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
}

void moveBackward() {
    currentMovement = BACKWARD;
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
}

void turnLeft() {
    currentMovement = LEFT;
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
}

void turnRight() {
    currentMovement = RIGHT;
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
}

void stopMotors() {
    currentMovement = STOPPED;
    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
}