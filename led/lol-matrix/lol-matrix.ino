// based on https://github.com/jprodgers/LoLshield/tree/master/examples/Animations/LoLShield_CharliHeart
// which appears to come from the book
// Arduino - Physical Computing für Bastler, Designer und Geeks
// by Manuel Odendah, Julian Finn, Alex Wenge

#include <Charliplexing.h>

#define MAX_SPEED 16
#define MIN_SPEED 3


struct point {
  uint8_t position;    // current position in x axis (multplied by 16)
  uint8_t speed;       // how fast point moves in y axis
  uint8_t brightness;  // 0-7 how bright are the leds for this column
  boolean is_on;       // switch between off (blank) line and an on line)
} points[9];

void setup() {
  Serial.begin(9600);
  LedSign::Init(GRAYSCALE);

  for (uint8_t i = 0; i < 9; i++) {
    reset_point(&points[i], random(2));
  }
}

void reset_point(point *p, boolean on) {
  p->position = 0;
  p->is_on = on;
  p->speed = (on ? random(MIN_SPEED, MAX_SPEED) : MAX_SPEED);
  p->brightness = (on ? random(1, 4) : 0);
}


void loop() {
  for (uint8_t i = 0; i < 9; i++) {
    point *p = &points[i];
    p->position += p->speed;

    // reset a point when it reaches the bottom of the screen
    if (p->position >= 14 * MAX_SPEED) {
      reset_point(p, ! p->is_on);
    }

    LedSign::Set(p->position / MAX_SPEED, i, p->brightness);
  }

  delay(100);
}
