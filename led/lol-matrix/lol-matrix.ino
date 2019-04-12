// based on https://github.com/jprodgers/LoLshield/tree/master/examples/Animations/LoLShield_CharliHeart
// which appears to come from the book
// Arduino - Physical Computing für Bastler, Designer und Geeks
// by Manuel Odendah, Julian Finn, Alex Wenge

#include <Charliplexing.h>

#define MAX_SPEED 16
#define MIN_SPEED 1


struct point {
  uint8_t position;    // current position in x axis (multplied by 16)
  uint8_t speed;       // how fast point moves in x axis
  uint8_t brightness;  // 0-7 how bright are the leds for this column
  boolean is_on;       // switch between off (blank) line and an on line)
} points[9];

void setup() {
  Serial.begin(115200);
  LedSign::Init(GRAYSCALE);

  for (uint8_t i = 0; i < 9; i++) {
    uint8_t brightness = random(2) * random(SHADES);
    init_point(&points[i], brightness);
  }
}

void init_point(point *p, uint8_t brightness) {
  p->position = 0;
  p->speed = random(MIN_SPEED, MAX_SPEED);
  p->is_on = true;
  p->brightness = random(2) * random(2, SHADES);;
}

void loop() {
  for (uint8_t i = 0; i < 9; i++) {
    point *p = &points[i];
    p->position += p->speed;
    Serial.println(p->position);

    // reached the bottom of the screen
    if (p->position >= 14 * 16) {
      init_point(p, 0);
    }

    LedSign::Set(p->position / 16, i, p->brightness);
  }

  delay(40);
}
