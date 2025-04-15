#include "leds.h"

const int Led = 23; // LED pin

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