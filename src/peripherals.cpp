#include "peripherals.hpp"

void Peripherals::Setup()
{
    oneWire1 = OneWire(TEMP_PIN_1);
    oneWire2 = OneWire(TEMP_PIN_2);
    oneWire3 = OneWire(TEMP_PIN_3);

    tempSensor1 = DallasTemperature(&oneWire1);
    tempSensor2 = DallasTemperature(&oneWire2);
    tempSensor3 = DallasTemperature(&oneWire3);

    tempSensor1.begin();
    tempSensor2.begin();
    tempSensor3.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

float Peripherals::GetTemperature(int sensorID, bool log)
{
    DallasTemperature tempSensor;
    switch (sensorID)
    {
    case 0:
        tempSensor = tempSensor1;
        break;
    case 1:
        tempSensor = tempSensor2;
        break;
    case 2:
        tempSensor = tempSensor3;
        break;
    
    default:
        Serial.println("Temperature sensor with ID " + String(sensorID) + " does not exist");
        return 0.0f;
    }

    float temperature = 0;
    for (size_t i = 0; i < 5; i++)
    {
        tempSensor.requestTemperatures();
        temperature = tempSensor.getTempCByIndex(0);

        if(temperature >= -100 && temperature <= 60 && temperature != 0)
            break;
    }
    
    if(log)
    {
        Serial.print("Temperature " + String(sensorID) + ": " + String(temperature) + " ");
    }

    return temperature;
}
