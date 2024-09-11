#ifndef PERIPHERALS_H
#define PERIPHERALS_H

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

OneWire oneWire1;
OneWire oneWire2;
OneWire oneWire3;

DallasTemperature tempSensor1;
DallasTemperature tempSensor2;
DallasTemperature tempSensor3;

public:
    void Setup();
    float GetTemperature(int sensorID, bool log = true);
};

#endif