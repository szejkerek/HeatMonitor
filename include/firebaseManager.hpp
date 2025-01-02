#pragma once

#include <Arduino.h>

struct CustomData {
  float temperature1;
  float temperature2;
  float temperature3;
};

String formatTimestamp(unsigned long timestamp);
void setupFirebase(const String& apiKey, const String& databaseUrl);
bool saveDataToFirebase(const CustomData& data, unsigned long timestamp);
