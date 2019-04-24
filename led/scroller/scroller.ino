#include <LedControl.h>
#include <string.h>

LedControl two = LedControl( 4, 6, 5, 8); // data (blue), clock (green), select (orange), 8 displays
LedControl one = LedControl( 7, 9, 8, 8); // data (blue), clock (green), select (orange), 8 displays

byte sprites[2][2][8] = {
  {
    //{0x42, 0x81, 0x5A, 0xFF, 0xDB, 0x7E, 0x3c, 0x18}, {0xA5, 0x5A, 0x24, 0xFF, 0xDB, 0x7E, 0x3c, 0x18} // squid
    {0x18, 0x3c, 0x7E, 0xDB, 0xFF, 0x5A, 0x81, 0x42}, {0x18, 0x3c, 0x7E, 0xDB, 0xFF, 0x24, 0x5A, 0xA5} // squid
  },
  {
    {0x18, 0xA5, 0xFF, 0xFF, 0xDB, 0x7E, 0x24, 0x42}, {0x42, 0x24, 0x7E, 0xFF, 0xDB, 0xFF, 0xA5, 0x42} // happy
  }
};

void setup() {
  for (int i = 0; i < one.getDeviceCount(); i++) {
    one.shutdown(i, false);
    one.setIntensity(i, 1);
    one.clearDisplay(i);
  }
}

void show_sprite_at_x(byte x, byte sprite[8]) {
  byte matrix = x / 8;
  byte offset = x % 8;
  for (byte row = 0; row < 8; row++) {
    one.spiTransfer(matrix - 1 , row + 1, sprite[row] << offset);
    one.spiTransfer(matrix , row + 1, sprite[row] >> (8 - offset));
    if (offset == 0) {
      one.clearDisplay(matrix - 2);
    }
  }
}

void loop() {
  for (byte i = 0; i < 100; i++) {
    show_sprite_at_x(i, sprites[0][0]);
    delay(100);
  }
}

