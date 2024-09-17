#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <queue>
#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor
{
    const int validSeriesCount = 5;
    std::queue<float> tempReadings;
    OneWire oneWire;
    DallasTemperature tempSensor;

public:
    TemperatureSensor(int pinID);

    void RequestTemperature();
    float GetTemperature();
};

#endif
