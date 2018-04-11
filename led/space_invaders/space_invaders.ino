#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 5, 4, 6, 4); // data, clock, device select

byte sprites[2][2][8] = {
  {
    {165, 90, 36, 255, 219, 126, 60, 24}, {66, 90, 36, 255, 219, 126, 60, 24} // squid
  },
  {
    {36, 165, 255, 255, 219, 126, 36, 36}, {66, 36, 126, 255, 219, 255, 165, 36} // happy
  }
};

#define SWITCH_PIN 2
#define BOARD_LED 13


byte sprite_index = 0;

volatile boolean switch_triggered = false;

void setup() {

  // initialise all led matrices
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(i, false);
    /* Set the brightness 0 - 15 */
    lc.setIntensity(i, 1);
    /* and clear the display */
    lc.clearDisplay(i);
  }

  // attach interrupt function, click(), to the micro switch
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), click, RISING);
  pinMode(BOARD_LED, OUTPUT);
}

void click() {
  switch_triggered = true;
  digitalWrite(BOARD_LED, HIGH);
}


void show_sprite_at(byte location, byte sprite[2][8], byte toggle) {
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(location, row + 1, sprite[toggle][row]);
  }
}


void ___show_sprite_at_x(byte x, byte sprite[2][8], byte toggle) {
  byte addr = (31 - x) / 8;
  byte offset = x % 8;
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(addr, row + 1, sprite[toggle][row] >> offset);
    lc.spiTransfer(addr - 1, row + 1, sprite[toggle][row] << (8 - offset));
    lc.clearDisplay(addr + 1);
  }
}

void show_sprite_at_x(byte x, byte sprite[8]) {
  byte matrix = x / 8;
  byte offset = x % 8;
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(matrix, row + 1, sprite[row] << offset);
    lc.spiTransfer(matrix + 1, row + 1, sprite[row] >> (8 - offset));
    lc.clearDisplay(matrix - 1);
  }
}

void loop() {
  if (switch_triggered) {
    run_animation();
    switch_triggered = false;
  }
  delay(20);
}

void run_animation() {

  for (byte x = 0; x < 33; x++) {
    show_sprite_at_x(x, sprites[sprite_index][ x % 2]);
    delay(500);
    digitalWrite(BOARD_LED, LOW);

  }
  sprite_index = (sprite_index + 1) % 2;
}
