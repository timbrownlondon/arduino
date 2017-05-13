#include <LiquidCrystal.h>

//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  lcd.begin(16, 1);
  lcd.print("hello, world!");
}

void loop() {}
