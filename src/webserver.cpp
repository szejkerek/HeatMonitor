#include "webserver.hpp"
#include <algorithm>
#include <WiFi.h>

// Create WebServer object on port 80
WebServer server(80);

std::vector<float> temp1Readings, temp2Readings, temp3Readings, distanceReadings;

const size_t WINDOW_SIZE = 10;  // Reduce window size for memory efficiency

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

// Handle root path
void handleRoot() {
    String html = "<!DOCTYPE html><html>";
    html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<meta http-equiv='refresh' content='10'>"; // Auto-refresh every 10 seconds
    html += "<title>Sensor Dashboard</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f5f5f5; }";
    html += ".container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
    html += "h1 { color: #333; text-align: center; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    html += "th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }";
    html += "th { background-color: #4CAF50; color: white; }";
    html += "tr:hover { background-color: #f5f5f5; }";
    html += ".footer { margin-top: 20px; text-align: center; font-size: 12px; color: #777; }";
    html += "</style></head>";
    html += "<body><div class='container'>";
    html += "<h1>Sensor Readings</h1>";
    html += "<table>";
    html += "<tr><th>Sensor</th><th>Current Value</th><th>Median Value</th></tr>";
    
    // Temperature 1
    html += "<tr><td>Temperature 1</td><td>";
    html += temp1Readings.size() > 0 ? String(temp1Readings.back(), 2) + " °C" : "No data";
    html += "</td><td>";
    html += String(getMedian(temp1Readings), 2) + " °C</td></tr>";
    
    // Temperature 2
    html += "<tr><td>Temperature 2</td><td>";
    html += temp2Readings.size() > 0 ? String(temp2Readings.back(), 2) + " °C" : "No data";
    html += "</td><td>";
    html += String(getMedian(temp2Readings), 2) + " °C</td></tr>";
    
    // Temperature 3
    html += "<tr><td>Temperature 3</td><td>";
    html += temp3Readings.size() > 0 ? String(temp3Readings.back(), 2) + " °C" : "No data";
    html += "</td><td>";
    html += String(getMedian(temp3Readings), 2) + " °C</td></tr>";
    
    // Distance
    html += "<tr><td>Distance</td><td>";
    html += distanceReadings.size() > 0 ? String(distanceReadings.back(), 2) + " cm" : "No data";
    html += "</td><td>";
    html += String(getMedian(distanceReadings), 2) + " cm</td></tr>";
    
    html += "</table>";
    
    // System info
    html += "<h2>System Info</h2>";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Value</th></tr>";
    html += "<tr><td>Device IP</td><td>" + WiFi.localIP().toString() + "</td></tr>";
    html += "<tr><td>WiFi RSSI</td><td>" + String(WiFi.RSSI()) + " dBm</td></tr>";
    html += "<tr><td>Uptime</td><td>" + String(millis() / 1000 / 60) + " minutes</td></tr>";
    html += "<tr><td>Free Heap</td><td>" + String(ESP.getFreeHeap() / 1024) + " KB</td></tr>";
    html += "</table>";
    
    html += "<div class='footer'>Last update: " + String(millis() / 1000) + "s since boot</div>";
    html += "</div></body></html>";
    
    server.send(200, "text/html", html);
}

// Handle not found
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    
    server.send(404, "text/plain", message);
}

// Handle JSON API endpoint
void handleAPI() {
    String json = "{";
    json += "\"temp1\":" + String(getMedian(temp1Readings), 2) + ",";
    json += "\"temp2\":" + String(getMedian(temp2Readings), 2) + ",";
    json += "\"temp3\":" + String(getMedian(temp3Readings), 2) + ",";
    json += "\"distance\":" + String(getMedian(distanceReadings), 2) + ",";
    json += "\"uptime\":" + String(millis() / 1000) + ",";
    json += "\"rssi\":" + String(WiFi.RSSI());
    json += "}";
    
    server.send(200, "application/json", json);
}

void setupWebServer() {
    // Set up web server routes
    server.on("/", handleRoot);
    server.on("/api", handleAPI);
    server.onNotFound(handleNotFound);
    
    // Start the server
    server.begin();
    Serial.println("HTTP server started");
}

void handleWebClients() {
    server.handleClient();  // Handle web server clients
}

void updateSensorReadings(float temp1, float temp2, float temp3, float distance) {
    addReading(temp1Readings, temp1);
    addReading(temp2Readings, temp2);
    addReading(temp3Readings, temp3);
    addReading(distanceReadings, distance);
}

float getMedianTemp1() {
    return getMedian(temp1Readings);
}

float getMedianTemp2() {
    return getMedian(temp2Readings);
}

float getMedianTemp3() {
    return getMedian(temp3Readings);
}

float getMedianDistance() {
    return getMedian(distanceReadings);
}