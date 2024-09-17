#include "peripherals.hpp"
#include "WebServerManager.hpp"

Peripherals peripherals;
WebServerManager webServer(&peripherals);

void setup()
{
  Serial.begin(9600);
  webServer.begin();
}

void loop()
{
  webServer.checkInternetConnection();

  peripherals.temperture1.RequestTemperature();
  peripherals.temperture1.GetTemperature();
  delay(3000);
}
