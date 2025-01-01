#pragma once

#include <Arduino.h>

struct CustomData {
  float temperature;
  float humidity;
};

String formatTimestamp(unsigned long timestamp);
void setupFirebase(const String& apiKey, const String& databaseUrl);
bool saveDataToFirebase(const CustomData& data, unsigned long timestamp);
