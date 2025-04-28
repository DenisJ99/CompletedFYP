#include "leds.h"

const int Led = 23;
LEDMode currentMode = LED_OFF;

const int warningDistance = 30; 
const int criticalDistance = 15; 

void setupLED() {
    pinMode(Led, OUTPUT);
    digitalWrite(Led, LOW);
}

void alertLED(bool fast) {
    int delayTime = fast ? 100 : 500;
    digitalWrite(Led, HIGH);
    delay(delayTime);
    digitalWrite(Led, LOW);
    delay(delayTime);
}

void setLEDMode(LEDMode mode) {
    currentMode = mode;
    if (mode == LED_OFF) {
        digitalWrite(Led, LOW);
    } else if (mode == LED_ON) {
        digitalWrite(Led, HIGH);
    }
}

void updateLEDs(int distance1, int distance2, int distance3) {
    if (currentMode != LED_ULTRASONIC) return;

    if (distance1 < criticalDistance || distance2 < criticalDistance || distance3 < criticalDistance) {
        alertLED(true); 
    }
    else if (distance1 < warningDistance || distance2 < warningDistance || distance3 < warningDistance) {
        alertLED(false); 
    }
    else {
        digitalWrite(Led, LOW); 
    }
}