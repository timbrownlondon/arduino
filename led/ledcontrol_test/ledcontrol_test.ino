#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 5, 7, 6, 4);

boolean pix_on = true;

unsigned long delaytime = 1;

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

void loop() {
  //read the number cascaded devices
  int devices = lc.getDeviceCount();

  //we have to init all devices in a loop
  for (int address = 0; address < devices; address++) {
    for (int col = 7; col >= 0; col--) {

      for (int row = 0; row < 8; row++) {
        lc.setLed(address, row, col, pix_on);

        // delay(delaytime);
        // delay(delaytime);
        // lc.setLed(address,row,col,false);
      }
    }
  }
  pix_on = ! pix_on;
}
