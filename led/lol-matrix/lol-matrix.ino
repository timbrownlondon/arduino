// based on https://github.com/jprodgers/LoLshield/tree/master/examples/Animations/LoLShield_CharliHeart
// which appears to come from the book
// Arduino - Physical Computing für Bastler, Designer und Geeks
// by Manuel Odendah, Julian Finn, Alex Wenge

#include <Charliplexing.h>

struct point {
  uint8_t position; // current position in x axis (multplied by 16)
  uint8_t speed;    // how fast point moves in x axis
  boolean is_on;    // switch between off (blank) line and an on line)
} points[9];

void setup() {
  LedSign::Init(GRAYSCALE);

  for (uint8_t i = 0; i < 9; i++) {
    points[i].position = random(14);
    points[i].speed = random(3, 16);
    points[i].is_on = true;
  }
}


void loop() {
  for (uint8_t i = 0; i < 9; i++) {
    points[i].position += points[i].speed;
    if (points[i].position >= 14 * 16) {
      points[i].position = 0;
      points[i].speed = random(3, 16);
      points[i].is_on = not points[i].is_on;
    }
    uint8_t brightness = (points[i].is_on ? random(1, SHADES) : 0);
    LedSign::Set(points[i].position / 16, i, brightness);
  }

  delay(50);
}
