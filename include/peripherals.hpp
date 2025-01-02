#pragma once

#include "temperatureSensor.hpp"
#include <OneWire.h>
#include <DallasTemperature.h>

const int TEMP_PIN_1 = 18;
const int TEMP_PIN_2 = 19;
const int TEMP_PIN_3 = 21;
const int BUTTON_PIN = 34;
const int LED_PIN = 13;

// TemperatureSensor temperature1(TEMP_PIN_1);
// TemperatureSensor temperature2(TEMP_PIN_2);
// TemperatureSensor temperature3(TEMP_PIN_3);

void setupPeripherals()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button pin
    pinMode(LED_PIN, OUTPUT);          // Configure LED pin
    digitalWrite(LED_PIN, LOW);        // Ensure LED is off initially
}

void SetDiodeState(bool turnOn)
{
    digitalWrite(LED_PIN, turnOn ? HIGH : LOW);
}
