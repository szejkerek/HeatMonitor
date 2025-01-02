#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <queue>
#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor
{
    OneWire oneWire;
    DallasTemperature tempSensor;
    unsigned long lastRequestTime;

public:
    TemperatureSensor(int pinID);

    void RequestTemperature();
    float GetTemperature();
};

#endif
