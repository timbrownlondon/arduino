// using LED & Key board as display
#include <TM1638.h>

// attach middle pin of TMP36 to analog input pin 0
// attache others to 5V and GND
int sensorPin = 0;

// set pins for display
TM1638 module(8, 9, 10, true, 0);

float temperatures[10];
int n = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int reading = analogRead(sensorPin);

  float voltage = reading * 5.0;
  voltage /= 1024.0;

  int degreesC = round((voltage - 0.5) * 1000.0);
  Serial.println(degreesC);

  n = (n + 1) %10;
  Serial.println(n);

  temperatures[n] = degreesC;

  module.setDisplayToDecNumber(mean_temperature(), 2, false);

  delay(1000);
}

int mean_temperature() {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += temperatures[i];
  }
  return round(sum / 10);
}


