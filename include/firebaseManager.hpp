#pragma once

#include <Arduino.h>

struct CustomData {
  float temperature1;
  float temperature2;
  float temperature3;
  float distance;
};

String formatTimestamp(unsigned long timestamp);
bool setupFirebase(const String& apiKey, const String& databaseUrl);
bool saveDataToFirebase(const CustomData& data, unsigned long timestamp);

String getFirebaseData();
