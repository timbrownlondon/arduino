// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>

#include <DateDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 mid_board(8, 9, 6, true, 0);
TM1638 top_board(8, 9, 7, true, 0);

DateDisplay date_top(&top_board, 4, 4, 1962, true); // is_forwards = true
DateDisplay date_mid(&mid_board, 6, 3, 1999, false);

void setup() {
}

void loop() {
  date_top.processButtons();
  date_top.update();
  date_top.display();

  date_mid.processButtons();
  date_mid.update();
  date_mid.display();
}

