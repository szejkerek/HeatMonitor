#pragma once
#include <Arduino.h>
#include <vector>

class DistanceSensor {
    private:
        const int ECHO = 26;
        const int TRIGGER = 25;
        static constexpr float SOUND_SPEED = 0.034; // Speed of sound in cm/us
    
        float GetDistanceRaw() {
            digitalWrite(TRIGGER, LOW);
            delayMicroseconds(2);
            digitalWrite(TRIGGER, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIGGER, LOW);
            
            return pulseIn(ECHO, HIGH) * SOUND_SPEED / 2;
        }
    
    public:
        void Init() {
            pinMode(TRIGGER, OUTPUT);
            pinMode(ECHO, INPUT);
        }
    
        float GetDistance(int count = 15) {
            std::vector<float> distances;
            for (int i = 0; i < count; i++) {
                float distance = GetDistanceRaw();
                if (distance > 500 || distance < 0.1) continue;
                distances.push_back(distance);
            }
            if (distances.empty()) return -1;
            std::sort(distances.begin(), distances.end());
            size_t size = distances.size();
            return size % 2 == 0 ? (distances[size / 2 - 1] + distances[size / 2]) / 2.0 : distances[size / 2];
        }
    };