#include "buzzer.h"

const int buzzerPin = 19;
const int buzzerChannel = 0;
const int buzzerFrequency = 1000;
const int buzzerResolution = 8;

void setupBuzzer() {
    ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);
    ledcWrite(buzzerChannel, 0); // Turn off buzzer initially
}

void alertBuzzer(bool fast) {
    int delayTime = fast ? 100 : 500;
    //ledcWrite(buzzerChannel, 128); // Turn on buzzer
    delay(delayTime);
    ledcWrite(buzzerChannel, 0); // Turn off buzzer
    delay(delayTime);
}