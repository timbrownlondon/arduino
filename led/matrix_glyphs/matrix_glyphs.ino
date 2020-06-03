#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 3, 4, 2, 2);

boolean pix_on = true;

unsigned long delaytime = 2000;

byte sprites[4][8] = {
  {0x42, 0x81, 0x5A, 0xFF, 0xDB, 0x7E, 0x3c, 0x18}, // squid A
  {0xA5, 0x5A, 0x24, 0xFF, 0xDB, 0x7E, 0x3c, 0x18}, // squid B
  {0x18, 0xA5, 0xFF, 0xFF, 0xDB, 0x7E, 0x24, 0x42}, // happy A
  {0x42, 0x24, 0x7E, 0xFF, 0xDB, 0xFF, 0xA5, 0x42}  // happy B
};

void show_sprite(int address, byte sprite[8]) {
  for (byte row = 0; row < 8; row++) {
    lc.setColumn(address, row, sprite[row]);
  }
}



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
    show_sprite(address, sprites[1]);
  }
}

void loop() {}

/*
  //read the number cascaded devices
  int devices = lc.getDeviceCount();

  show_sprite(1, sprites[1]);

  for (int count = 0; count < 2; count++) {
    show_sprite(count % 2, sprites[count]);

    // lc.setLed(address, row, col, pix_on);
    delay(delaytime);


  }
}
*/
