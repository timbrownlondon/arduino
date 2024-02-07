#include <Charliplexing.h>

byte cell[DISPLAY_COLS][DISPLAY_ROWS];

byte chance = 8; // one in 8 cells are likely to be on

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
  for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = pix();
      cell[x][y] = 0;
    }
  }
}

void loop() {
  for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
    for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
      // not sure if following line is needed (merge conflict)
      // LedSign::Set(x, y, cell[DISPLAY_COLS - (x + 1)][y]);
      if (x == DISPLAY_COLS - 1) {
        cell[x][y] = pix();
      }
      else {
        cell[x][y] = cell[x + 1][y];
      }
      LedSign::Set(x, y, cell[DISPLAY_COLS - x - 1][y]);
    }
  }
  delay(1400);
}

byte pix() {
<<<<<<< HEAD
  if (random(10)) return 0;
  return random(3);
=======
  if (random(chance)) return 0;
  return random(4) + 1;
>>>>>>> 69814a317f661461d211399da63f40fa76f25cb4
}
