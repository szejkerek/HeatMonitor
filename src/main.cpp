#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.hpp"
#include "peripherals.hpp"

Peripherals peripherals;

void setup() {
  Serial.begin(9600);  
  peripherals.Setup();
}

void loop() {
  peripherals.GetTemperature(0);
  peripherals.GetTemperature(1);
  peripherals.GetTemperature(2);
  Serial.print("\n");

  // buttonState = digitalRead(BUTTON_PIN);

  // Serial.print("Button: ");
  // Serial.print(buttonState);

  // Serial.print("\n");
  // if (buttonState == HIGH && lastButtonState == LOW) {
  //   diodeState = !diodeState;
  //   digitalWrite(LED_PIN, diodeState);
  // }
  // lastButtonState = buttonState;

  delay(1000);
}
