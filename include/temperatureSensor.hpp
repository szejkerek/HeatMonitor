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

TemperatureSensor::TemperatureSensor(int pinID)
    : oneWire(pinID), tempSensor(&oneWire)
{
    tempSensor.begin(); 
}

inline void TemperatureSensor::RequestTemperature()
{
    float temperature = 0;
    for (size_t i = 0; i < 5; i++)
    {
        tempSensor.requestTemperatures();
        temperature = tempSensor.getTempCByIndex(0);

        if (temperature >= -100 && temperature <= 60 && temperature != 0)
            break;
    }
    
    Serial.print("Temperature request: " + String(temperature) + " ");

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
    

    Serial.print("Temperature value: " + String((float)validSeriesCount) + " ");
    return sum / (float)validSeriesCount;
}

#endif
