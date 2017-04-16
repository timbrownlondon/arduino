#include <TM1638.h>


// define a module on data pin 8, clock pin 9 and strobe pin 10, activate is true, brightness 1
TM1638 module(8, 9, 10, true, 1);
unsigned long a = 1;

void setup() {
  byte values[] = { 1, 2, 4, 8, 16, 32, 64, 255, 0 };
  module.setDisplay(values);
  delay(400);

  module.setDisplayToHexNumber(0x1234ABCD, 0xF0);
  module.setLED(1, 1);
  module.setLEDs(255);
  delay(400);
}

void loop() {
  module.setDisplayToString("21 Oct 1985");
  delay(5000);
  for (a = 1; a <= 100000000; a++) {
    module.setDisplayToDecNumber(a, 0, true);
    module.setLEDs(0);
    module.setLED(1, a % 8);
    delay(400);
  }
}

