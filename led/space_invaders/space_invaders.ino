#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 5, 7, 6, 4);

byte sprites[2][2][8] = {
  {
    {24, 60, 126, 219, 255, 36, 90, 165}, {24, 60, 126, 219, 255, 36, 90, 66} // squid
  },
  {
    {36, 36, 126, 219, 255, 255, 165, 36}, {36, 165, 255, 219, 255, 126, 36, 66} // happy
  }
};


unsigned long delays[] = {150, 120, 96, 77, 62, 50, 40, 32, 26, 21, 17, 14, 11, 9, 7, 5, 4 , 3};
byte speed = 0; // slowest

byte toggle = 0;
byte x = 0;
byte direction = 1;
byte sprite_index = 0;

void setup() {
  int devices = lc.getDeviceCount();
  // init all devices
  for (int address = 0; address < devices; address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address, false);
    /* Set the brightness 0 - 15 */
    lc.setIntensity(address, 1);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}


void show_sprite_at(byte location, byte sprite[2][8], byte toggle) {
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(location, row + 1, sprite[toggle][row]);
  }
}


void show_sprite_at_x(byte x, byte sprite[2][8], byte toggle) {
  byte addr = (31 - x) / 8;
  byte offset = x % 8;
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(addr, row + 1, sprite[toggle][row] >> offset);
    lc.spiTransfer(addr - 1, row + 1, sprite[toggle][row] << (8 - offset));
    lc.clearDisplay(addr + 1);
  }
}


void loop() {
  show_sprite_at_x(x, sprites[sprite_index], toggle);
  delay(delays[speed]);

  toggle = (toggle + 1) % 2;
  x += direction;

  if (x == 0 or x == 24) {
    direction *= -1;
  }
  if (x == 0) {
    sprite_index = (sprite_index + 1) % 2;
    speed = (speed + 1) % 18;
  }
}
