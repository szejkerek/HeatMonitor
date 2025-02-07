#pragma once
#include <Arduino.h>
#include <vector>

class DistanceSensor
{
private:
    const int ECHO = 26;
    const int TRIGGER = 25;
    static constexpr float SOUND_SPEED = 0.034; // Speed of sound in cm/us

    float GetDistanceRaw()
    {
        long duration;
        float distanceCm;

        digitalWrite(TRIGGER, LOW);
        delayMicroseconds(2);

        digitalWrite(TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER, LOW);

        duration = pulseIn(ECHO, HIGH);

        distanceCm = duration * SOUND_SPEED / 2;

        return distanceCm;
    }


public:
    void Init()
    {
        pinMode(TRIGGER, OUTPUT);
        pinMode(ECHO, INPUT);
    }

void GetDistance(int count = 15)
{
    std::vector<float> distances;
    bool invalidMeasurement = false;

    // Collect measurements
    for (int i = 0; i < count; i++)
    {
        float distance = GetDistanceRaw();
        if (distance > 500 || distance < 0.1)
        {
            return;
        }

        distances.push_back(distance);
        delay(50);
    }

    std::sort(distances.begin(), distances.end());
    float median = 0.0;
    if (count % 2 == 0)
    {
        median = (distances[count / 2 - 1] + distances[count / 2]) / 2.0;
    }
    else
    {
        median = distances[count / 2];
    }

    Serial.println(median);
}
    
};
