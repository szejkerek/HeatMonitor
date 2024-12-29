#include "peripherals.hpp"
#include "WebServerManager.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <HTTPClient.h>
#include <UrlEncode.h>


// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void sendMessage(String message) {
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  bool messageSent = false; // Flag to check if the message was sent successfully
  int maxRetries = 3;       // Maximum number of retries
  int attempt = 0;          // Counter for the number of attempts

  while (!messageSent && attempt < maxRetries) {
    attempt++;
    http.begin(url);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send HTTP POST request
    int httpResponseCode = http.POST(url);
    if (httpResponseCode == 200) {
      Serial.println("Message sent successfully");
      messageSent = true;
    } else {
      Serial.println("Error sending the message");
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Retrying... Attempt ");
      Serial.println(attempt);
    }

    // Free resources
    http.end();
  }

  if (!messageSent) {
    Serial.println("Failed to send the message after multiple attempts.");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the API key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign-up successful");
    signupOK = true;
  } else {
    Serial.printf("Error: %s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(34, INPUT_PULLUP); // Set button pin as input with pull-up
}

void loop() {
  if (digitalRead(34) == LOW) { // Check if the button is pressed
    delay(50); // Debounce delay
    if (digitalRead(34) == LOW) { // Confirm button press
      sendMessage("Clicked"); // Send "Clicked" message
      Serial.println("Button pressed, message sent.");
      while (digitalRead(34) == LOW); // Wait for button release
    }
  }

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Simulate temperature data
    float temperature = 20.0 + random(0, 100) / 10.0; // Random temperature for demonstration

    // Get timestamp in milliseconds
    unsigned long timestamp = millis(); 

    // Construct database path
    String path = "temperature_data/" + String(timestamp);

    // Save temperature data to the database
    if (Firebase.RTDB.setFloat(&fbdo, path, temperature)) {
      Serial.println("Temperature data written to Firebase:");
      Serial.println("Timestamp: " + String(timestamp));
      Serial.println("Temperature: " + String(temperature));
    } else {
      Serial.println("FAILED to write data");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
