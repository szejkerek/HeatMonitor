#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "temperatureSensor.hpp"
#include <OneWire.h>
#include <DallasTemperature.h>

class Peripherals
{
private:
const int TEMP_PIN_1 = 18;
const int TEMP_PIN_2 = 19;
const int TEMP_PIN_3 = 21;
const int BUTTON_PIN = 34;
const int LED_PIN = 13;

public:
    TemperatureSensor temperture1;
    TemperatureSensor temperture2;
    TemperatureSensor temperture3;

    Peripherals():temperture1(TEMP_PIN_1), temperture2(TEMP_PIN_2), temperture3(TEMP_PIN_3) 
    {
        pinMode(BUTTON_PIN, INPUT_PULLUP);
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(LED_PIN, LOW); 
    }

    void SetDiodeState(bool turnOn)
    {
        turnOn ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
    }
};

#endif