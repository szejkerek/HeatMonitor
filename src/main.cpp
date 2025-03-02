#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <algorithm>

#include "config.hpp"
#include "peripherals.hpp"
#include "serviceManager.hpp"
#include "firebaseManager.hpp"
#include "whatsappMessaging.hpp"
#include "webserver.hpp"  // Add the new header

const unsigned long READING_INTERVAL = 60000 / 10;  // 10 readings per minute
unsigned long lastReadingTime = 0;

void sendDataTask(void* parameter) {
    while (true) {
        vTaskDelay(60000 / portTICK_PERIOD_MS);  // Wait 60s before next send
        if (WiFi.isConnected()) {
            float medianTemp1 = getMedianTemp1();
            float medianTemp2 = getMedianTemp2();
            float medianTemp3 = getMedianTemp3();
            float medianDistance = getMedianDistance();

            Serial.printf("Sending Data - Temp1: %.2f, Temp2: %.2f, Temp3: %.2f, Distance: %.2f\n",
                          medianTemp1, medianTemp2, medianTemp3, medianDistance);

            CustomData sensorData = {medianTemp1, medianTemp2, medianTemp3, medianDistance};
            unsigned long timestamp = millis();

            if (saveDataToFirebase(sensorData, timestamp)) {
                Serial.println("Data saved successfully.");
            } else {
                Serial.println("Failed to save data.");
            }
        }     
    }
}

void setup() {
    Serial.begin(9600);
    setupPeripherals();

    // Ensure WiFi connects before proceeding
    while (!checkConnectionAndTryReconnect()) {
        Serial.println("WiFi connection failed, retrying...");
        delay(5000);
    }
    
    Serial.println("Connected to WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize the web server
    setupWebServer();
    
    delay(2000);  // Allow network stack to stabilize

    xTaskCreatePinnedToCore(sendDataTask, "FirebaseTask", 8192, NULL, 1, NULL, 1);
}

void loop() {
    if (!checkConnectionAndTryReconnect()) return;  // Reconnect if disconnected
    
    handleWebClients();  // Handle web server clients

    if (millis() - lastReadingTime > READING_INTERVAL) {
        lastReadingTime = millis();

        float temp1 = temperature1.GetTemperature();
        float temp2 = temperature2.GetTemperature();
        float temp3 = temperature3.GetTemperature();
        float distance = distanceSensor.GetDistance();

        Serial.printf("Temp1: %.2f, Temp2: %.2f, Temp3: %.2f, Distance: %.2f\n", temp1, temp2, temp3, distance);

        updateSensorReadings(temp1, temp2, temp3, distance);
    }
}