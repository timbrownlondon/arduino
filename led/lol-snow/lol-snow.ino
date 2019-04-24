#include <Charliplexing.h>

byte cell[DISPLAY_COLS][DISPLAY_ROWS];

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
  for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (byte y = 0; y <= DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = pix();
    }
  }
}

boolean on = false;
byte chance = 5;
byte delta = 1;

void loop() {
  chance += delta;
  if(chance > 50){
    delta = -1;
  }
  if(chance < 5){
    delta = 1;
  }
  for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
    for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
      LedSign::Set(x, y, cell[x][y]);
      if (y == DISPLAY_ROWS - 1) {
        cell[x][y] = pix();
      }
      else {
        cell[x][y] = cell[x][y + 1];
      }
    }
  }
  delay(300);
}

byte pix() {
  if (random(chance)) return 0;
  return random(2) + 1;
}

