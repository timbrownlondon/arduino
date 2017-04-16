#include <TM1638.h>
#include <string.h>


// data pin 8, clock pin 9 and strobe pin 10, activate is true, brightness is 0-7
TM1638 module(8, 9, 10, true, 0);
unsigned long a = 1;
String str = "         26-Oct-1985   05-Apr-2015        ";

void setup() {
}

void loop() {
  module.setDisplayToString(str.substring(a++ % (str.length() - 8)));
  module.setLEDs(0);
  module.setLED(1, a % 8);
  delay(300);
}

