#include "LedControl.h"

/*
  LedControl( data_pin, clock_pin, select_device_pin, number_of_devices )
  a device is an 8 x 8 display controlled by a Max7219 chip
  typically they are chained together in up to 8 devices
*/

LedControl lc = LedControl( 5, 4, 6, 4); // data, clock, device select

byte sprites[2][2][8] = {
  {
    {0x42, 0x81, 0x5A, 0xFF, 0xDB, 0x7E, 0x3c, 0x18}, {0xA5, 0x5A, 0x24, 0xFF, 0xDB, 0x7E, 0x3c, 0x18} // squid
  },
  {
    {0x18, 0xA5, 0xFF, 0xFF, 0xDB, 0x7E, 0x24, 0x42}, {0x42, 0x24, 0x7E, 0xFF, 0xDB, 0xFF, 0xA5, 0x42} // happy
  }
};

int notes[] = {//31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65, 69, 73, 78, 82,
  /*
    F     F#     G   G#      A    A#   B    C    C#   D    D#   E
  */
  87,     93,   98,  104,  110,  117, 123, 131, 139, 147, 156, 165,
  175,   185,  196,  208,  220,  233, 247, 262, 277, 294, 311, 330,
  349,   370,  392,  415,  440,  466, 494, 523, 554, 587, 622, 659,
  698,   740,  784,  831,  880,  932, 988, 1047, 1109, 1175, 1245, 1319,
  1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637,
  2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978, 5257
};

#define SWITCH 2

byte sprite_index = 1;

volatile boolean switch_triggered = false;
boolean animate = false;
unsigned long last_click_millis = 0;
#define DEBOUNCE_MILLIS 800L

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
  pinMode(SWITCH, INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCH), click, RISING);

  Serial.begin(9600);
}

void click() {
  switch_triggered = true;
}


void show_sprite_at_x(byte x, byte sprite[8]) {
  byte matrix = x / 8;
  byte offset = x % 8;
  for (byte row = 0; row < 8; row++) {
    lc.spiTransfer(matrix - 1 , row + 1, sprite[row] << offset);
    lc.spiTransfer(matrix , row + 1, sprite[row] >> (8 - offset));
    if (offset == 0) {
      lc.clearDisplay(matrix - 2);
    }
  }
}

// starting position of first sprite
byte x = 0;
// z (0 or 1) used to move legs and arms
byte z = 0;

void loop() {
  /*
    if (switch_triggered and ((millis() - last_click_millis) > DEBOUNCE_MILLIS)) {
    Serial.println("_______");
    Serial.println(millis() - last_click_millis);
    Serial.println(last_click_millis);
    animate = ! animate;
    last_click_millis = millis();
    switch_triggered = false;
    }
  */
  if (switch_triggered) {
    animate = true;
    switch_triggered = false;
  }
  if (animate) {
    tone(9, notes[random(6 * 12)]);
    x++;
  }
  else {
    noTone(9);
  }
  z = (z + 1) % 2;
  show_sprite_at_x(x, sprites[ sprite_index ][ z ]);
  delay(80);


  // reset animation
  if (x > 45) {
    x = 0;
    sprite_index = (sprite_index + 1) % 2;
    animate = false;
  }
}

/*
  void run_animation() {
  byte x = 0;
  while (x < 41) {
    show_sprite_at_x(x, sprites[sprite_index][ x % 2]);
    tone(9, notes[x]); //random(5 * 12)]);
    delay(70);

    if (switch_triggered) {
      switch_triggered = false;
      animate = ! animate;
    }

    if (animate) {
      x++;
    }
  }
  sprite_index = (sprite_index + 1) % 2;
  noTone(9);
  }
*/
