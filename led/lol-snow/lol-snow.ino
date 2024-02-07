#include <Charliplexing.h>

byte cell[DISPLAY_COLS][DISPLAY_ROWS];

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
  for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = pix();
    }
  }
}

void loop() {
  for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
    for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
      LedSign::Set(x, y, cell[DISPLAY_COLS - (x + 1)][y]);
      if (x == DISPLAY_COLS - 1) {
        cell[x][y] = pix();
      }
      else {
        cell[x][y] = cell[x + 1][y];
      }
    }
  }
  delay(300);
}

byte pix() {
  if (random(10)) return 0;
  return random(3);
}

