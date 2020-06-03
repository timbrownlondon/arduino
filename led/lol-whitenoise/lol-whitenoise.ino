#include <Charliplexing.h>

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
}
void loop() {
  for (byte x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (byte y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      LedSign::Set(x, y, random(4));          // 8 is max brighteness I think
    }
  }
  delay(20);
}

