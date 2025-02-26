#include "temperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(int pinID) : oneWire(pinID), tempSensor(&oneWire), lastRequestTime(0)
{
    tempSensor.begin();
}

float TemperatureSensor::GetTemperature()
{
    unsigned long currentTime = millis();

    // Ensure at least 2 seconds between readings
    if (currentTime - lastRequestTime < 2000)
    {
        Serial.println("Error: Reading requested too soon. Wait for 2 seconds.");
        return NAN;
    }

    lastRequestTime = currentTime;

    tempSensor.requestTemperatures();
    float temperature = tempSensor.getTempCByIndex(0);

    // Validate reading
    if (temperature > -40.0 && temperature < 115.0 && temperature != 85.0) // Adjust range for DS18B20
    {
        Serial.println("Temperature value: " + String(temperature));
        return temperature;
    }
    else
    {
        Serial.println("Error: Invalid temperature reading.");
        return 0.0f;
    }
}
