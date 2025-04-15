#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

// LED pin
extern const int Led;

// Function declarations
void setupLED();
void alertLED(bool fast);

#endif