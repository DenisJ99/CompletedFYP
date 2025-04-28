#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

extern const int buzzerPin;
extern const int buzzerChannel;
extern const int buzzerFreq;
extern const int buzzerResolution;

enum BuzzerMode {
    BUZZER_OFF,
    BUZZER_ON_ONESHOT,
    BUZZER_ULTRASONIC
};

extern BuzzerMode currentBuzzerMode;

void setupBuzzer();
void alertBuzzer(bool fast);
void setBuzzerMode(BuzzerMode mode);
void updateBuzzer(int distance1, int distance2, int distance3);

#endif