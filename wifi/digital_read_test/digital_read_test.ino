
#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(9600);
}

byte pin = 0;

void loop() {
  pin = (pin + 1) % 16;
  Serial.print(pin);

  for (byte i = 0; i < 20; i++) {
    Serial.print(digitalRead(pin));
    delay(500);
  }
  Serial.println("");
}


