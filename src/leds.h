#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

extern const int Led;

enum LEDMode {
    LED_OFF,
    LED_ON,
    LED_ULTRASONIC
};

extern LEDMode currentMode;

void setupLED();
void alertLED(bool fast);
void setLEDMode(LEDMode mode);
void updateLEDs(int distance1, int distance2, int distance3);

#endif