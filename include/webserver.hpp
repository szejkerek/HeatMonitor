#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <Arduino.h>
#include <WebServer.h>
#include <vector>

// Initialize the web server
void setupWebServer();

// Handle client requests in the main loop
void handleWebClients();

// Update sensor readings for the web server
void updateSensorReadings(float temp1, float temp2, float temp3, float distance);

// Get the median values for sensors
float getMedianTemp1();
float getMedianTemp2();
float getMedianTemp3();
float getMedianDistance();

#endif // WEBSERVER_HPP