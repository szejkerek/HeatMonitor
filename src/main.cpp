#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <algorithm>

#include "config.hpp"
#include "peripherals.hpp"
#include "serviceManager.hpp"
#include "firebaseManager.hpp"
#include "whatsappMessaging.hpp"

std::vector<float> temp1Readings;
std::vector<float> temp2Readings;
std::vector<float> temp3Readings;
std::vector<float> distanceReadings;

const size_t WINDOW_SIZE = 12; // Store last 12 readings (~1 min)
const unsigned long READING_INTERVAL = 60000 / WINDOW_SIZE;
unsigned long lastReadingTime = 0;
unsigned long lastSendTime = 0;

float getMedian(std::vector<float>& values) {
  if (values.size() < 3) return 0; // Ensure enough data

  std::sort(values.begin(), values.end());
  size_t size = values.size();
  return size % 2 == 0 ? (values[size / 2 - 1] + values[size / 2]) / 2.0 : values[size / 2];
}

void addReading(std::vector<float>& readings, float newValue) {
  readings.push_back(newValue);
  if (readings.size() > WINDOW_SIZE) {
    readings.erase(readings.begin());
  }
}

void setup() {
  Serial.begin(9600);
  setupPeripherals();

  while (!checkConnectionAndTryReconnect()) {
    Serial.println("Initial connection failed, retrying...");
    delay(5000);
  }
  Serial.println("Initial connection established.");
}

void loop() {
  if(!checkConnectionAndTryReconnect())
      return;
  
  if (millis() - lastReadingTime > READING_INTERVAL) {
    lastReadingTime = millis();
    
    float temp1 = temperature1.GetTemperature();
    float temp2 = temperature2.GetTemperature();
    float temp3 = temperature3.GetTemperature();
    float distance = distanceSensor.GetDistance();
    
    Serial.printf("Temp1: %.2f, Temp2: %.2f, Temp3: %.2f, Distance: %.2f\n", temp1, temp2, temp3, distance);
    
    addReading(temp1Readings, temp1);
    addReading(temp2Readings, temp2);
    addReading(temp3Readings, temp3);
    addReading(distanceReadings, distance);
  }

  if (millis() - lastSendTime > 60000) {
    lastSendTime = millis();
    
    float medianTemp1 = getMedian(temp1Readings);
    float medianTemp2 = getMedian(temp2Readings);
    float medianTemp3 = getMedian(temp3Readings);
    float medianDistance = getMedian(distanceReadings);
    
    Serial.printf("Sending Median Data - Temp1: %.2f, Temp2: %.2f, Temp3: %.2f, Distance: %.2f\n", 
                  medianTemp1, medianTemp2, medianTemp3, medianDistance);

    CustomData sensorData;
    sensorData.temperature1 = medianTemp1;
    sensorData.temperature2 = medianTemp2;
    sensorData.temperature3 = medianTemp3;
    sensorData.distance = medianDistance;
    unsigned long timestamp = millis();
    
    if (saveDataToFirebase(sensorData, timestamp)) {
      Serial.println("Data saved successfully.");
    } else {
      Serial.println("Failed to save data.");
    }
  }
}
