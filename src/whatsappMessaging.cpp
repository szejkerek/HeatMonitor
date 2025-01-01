#include "whatsappMessaging.hpp"
#include <HTTPClient.h>
#include <UrlEncode.h>

void sendMessage(String message, const String& phoneNumber, const String& apiKey) {
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
