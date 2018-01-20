#include <Charliplexing.h>

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
}
void loop() {
  for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (byte y = 0; y < DISPLAY_ROWS; y++) { // 10 rows
      LedSign::Set(x, y, random(SHADES));     // SHADES is 8 for GRAYSCALE
    }
  }
  delay(20);
}

