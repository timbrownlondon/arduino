#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <gamma.h>


Adafruit_NeoMatrix matrix
  = Adafruit_NeoMatrix(8, 8, 2,
                       NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
                       NEO_MATRIX_ROWS   + NEO_MATRIX_ZIGZAG,
                       NEO_GRB           + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextColor(colors[0]);
  matrix.fillScreen(colors[2]);
  matrix.show();
}

int x    = matrix.width();
int pass = 0;

void __loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Howdy"));
  if (--x < -36) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(1000);
}

void loop() {
  matrix.drawPixel(0, 0, colors[1]);
  matrix.drawPixel(1, 7, colors[2]);
  matrix.drawPixel(3, 6, colors[3]);
  matrix.show();

  for (int i = 0; i, 8; i++) {
    matrix.drawPixel(i, i, colors[0]);
    matrix.show();
    delay(2000);
  }
}
