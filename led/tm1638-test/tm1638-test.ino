// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>

// define 2 modules on data pin 8, clock pin 9 and strobe pin 6 and 7
// activate is true, brightness is 1
TM1638 display_a(8, 9, 6, true, 1);
TM1638 display_b(8, 9, 7, true, 1);

unsigned long a = 1;

void setup() {
  byte values[] = { 1, 2, 4, 8, 16, 32, 64, 255, 0 };
  display_b.setDisplay(values);
  delay(400);

  display_b.setDisplayToHexNumber(0x1234ABCD, 0xF0);
  display_b.setLED(1, 1);
  display_b.setLEDs(255);
  //delay(400);
}

void loop() {
  display_b.setDisplayToString("21 Oct 1985");
  delay(5000);
  for (a = 1; a <= 100000000; a++) {

    byte led_bit_map = a % 256;
    display_a.setLEDs(led_bit_map);
    display_b.setLEDs(256 - led_bit_map);
    
    display_a.setDisplayToDecNumber(led_bit_map, 0, false);
    display_b.setDisplayToDecNumber(256 - led_bit_map, 0, false);



    delay(400);
  }
}

