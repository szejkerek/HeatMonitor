#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_PIN_1 18
#define TEMP_PIN_2 19
#define TEMP_PIN_3 21
#define BUTTON_PIN 34
#define LED_PIN 13

OneWire oneWire1(TEMP_PIN_1);
OneWire oneWire2(TEMP_PIN_2);
OneWire oneWire3(TEMP_PIN_3);

DallasTemperature tempSensor1(&oneWire1);
DallasTemperature tempSensor2(&oneWire2);
DallasTemperature tempSensor3(&oneWire3);

bool buttonState = LOW;
bool lastButtonState = LOW;
bool diodeState = LOW;

void setup() {
  Serial.begin(9600);

  tempSensor1.begin();
  tempSensor2.begin();
  tempSensor3.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  tempSensor1.requestTemperatures();
  tempSensor2.requestTemperatures();
  tempSensor3.requestTemperatures();

  float temp1 = tempSensor1.getTempCByIndex(0);
  float temp2 = tempSensor2.getTempCByIndex(0);
  float temp3 = tempSensor3.getTempCByIndex(0);

  Serial.print("Temp1: ");
  Serial.print(temp1);
  Serial.print(" C, Temp2: ");
  Serial.print(temp2);
  Serial.print(" C, Temp3: ");
  Serial.print(temp3);
  Serial.print("\n");

  buttonState = digitalRead(BUTTON_PIN);
  Serial.print("Button: ");
  Serial.print(buttonState);
  Serial.print("\n");
  if (buttonState == HIGH && lastButtonState == LOW) {
    diodeState = !diodeState;
    digitalWrite(LED_PIN, diodeState);
  }
  lastButtonState = buttonState;

  delay(500);
}
