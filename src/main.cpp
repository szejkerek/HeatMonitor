#include "peripherals.hpp"
#include "WebServerManager.hpp"

Peripherals peripherals;          // Create peripherals object
WebServerManager webServer(&peripherals);  // Pass peripherals to the WebServerManager

void setup() {
  Serial.begin(9600);  
  webServer.begin();
}

void loop() {
  webServer.checkInternetConnection();

  peripherals.temperture1.RequestTemperature();
  peripherals.temperture1.GetTemperature();

  // peripherals.temperture2.RequestTemperature();
  // peripherals.temperture2.GetTemperature();

  // peripherals.temperture3.RequestTemperature();
  // peripherals.temperture3.GetTemperature();

  Serial.print("\n");
  delay(3000);
}
