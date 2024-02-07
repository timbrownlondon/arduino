#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 4, 5, 6, 8); // data, clock, device select, number of devices

void setup() {
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    /*The MAX719 is in power-saving mode on startup*/
    lc.shutdown(i, false);
    /* Set the brightness 0 - 15 */
    lc.setIntensity(i, 4);
    /* and clear the display */
    lc.clearDisplay(i);
  }

}

void loop() {
  for (byte a = 0; a < 8; a++) {
    for (byte i = 0; i < 8; i++) {
      lc.setDigit(a, i, i, false);
    }
  }
}

