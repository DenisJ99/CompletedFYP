#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// Buzzer pin and PWM settings
extern const int buzzerPin;
extern const int buzzerChannel;
extern const int buzzerFrequency;
extern const int buzzerResolution;

// Function declarations
void setupBuzzer();
void alertBuzzer(bool fast);

#endif