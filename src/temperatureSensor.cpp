#include "temperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(int pinID) : oneWire(pinID), tempSensor(&oneWire)
{
    tempSensor.begin();
}

void TemperatureSensor::RequestTemperature()
{
    float temperature = 0;
    for (size_t i = 0; i < 5; i++)
    {
        tempSensor.requestTemperatures();
        temperature = tempSensor.getTempCByIndex(0);

        if (temperature >= -100 && temperature <= 60 && temperature != 0)
            break;
    }

    tempReadings.push(temperature);

    if (tempReadings.size() > validSeriesCount)
        tempReadings.pop();
}

float TemperatureSensor::GetTemperature()
{
    if (tempReadings.size() < validSeriesCount)
    {
        Serial.println("Not enough temperature readings.");
        return NAN;
    }

    float sum = 0;
    std::queue<float> tempQueue = tempReadings;

    while (!tempQueue.empty())
    {
        sum += tempQueue.front();
        tempQueue.pop();
    }

    Serial.println("Temperature value: " + String(sum / (float)validSeriesCount) + " ");
    return sum / (float)validSeriesCount;
}