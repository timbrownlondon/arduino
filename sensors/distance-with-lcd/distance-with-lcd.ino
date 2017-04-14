#include <NewPing.h>
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define ECHO_PIN       2
#define TRIGGER_PIN    3
#define MAX_DISTANCE 500

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  lcd.begin(16, 2); //initialise lcd with dimensions
}

void loop() {
  delay(250);
  int distance = sonar.ping_cm();
  if (distance) {
    lcd.setCursor(5, 0);
    lcd.print(distance);
    lcd.print(" cm    ");
  }
}
