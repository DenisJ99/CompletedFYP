#include "buzzer.h"

const int buzzerPin = 19;
const int buzzerChannel = 0;
const int buzzerFreq = 1000;
const int buzzerResolution = 8;


BuzzerMode currentBuzzerMode = BUZZER_OFF;

const int buzzerWarningDistance = 10; // cm
const int buzzerCriticalDistance = 5; // cm

void setupBuzzer() {
    ledcSetup(buzzerChannel, buzzerFreq, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);
    ledcWrite(buzzerChannel, 0); 
}

void alertBuzzer(bool fast) {
    int delayTime = fast ? 100 : 500;
    int onDuration = fast ? 50 : 200;
    ledcWrite(buzzerChannel, 128); 
    delay(onDuration); 
    ledcWrite(buzzerChannel, 0); 
    delay(delayTime - onDuration); 
}

void setBuzzerMode(BuzzerMode mode) {
    currentBuzzerMode = mode;
    if (mode == BUZZER_OFF) {
        ledcWrite(buzzerChannel, 0); 
    } else if (mode == BUZZER_ON_ONESHOT) {
        ledcWrite(buzzerChannel, 128); 
        delay(1000); 
        ledcWrite(buzzerChannel, 0); 
        currentBuzzerMode = BUZZER_OFF; 
    }
 
}

void updateBuzzer(int distance1, int distance2, int distance3) {
    if (currentBuzzerMode != BUZZER_ULTRASONIC) {

        if (currentBuzzerMode == BUZZER_OFF) {
             ledcWrite(buzzerChannel, 0); 
        }
        return;
    }

    bool objectDetected = (distance1 <= buzzerWarningDistance || distance2 <= buzzerWarningDistance || distance3 <= buzzerWarningDistance);
    bool objectClose = (distance1 <= buzzerCriticalDistance || distance2 <= buzzerCriticalDistance || distance3 <= buzzerCriticalDistance);

    if (objectClose) {
        alertBuzzer(true); 
    } else if (objectDetected) {
        alertBuzzer(false); 
    } else {
        ledcWrite(buzzerChannel, 0); 
    }
}