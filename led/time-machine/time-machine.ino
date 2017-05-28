// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>

#include <DateDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 mid_board(8, 9, 6, true, 0);
TM1638 top_board(8, 9, 7, true, 0);

DateDisplay date_top(4, 4, 1962, 100, true);
DateDisplay date_mid(4, 4, 2032, 110, false);

void setup() {
}

void loop() {
  if (date_top.readyToUpdate(millis())) {
    date_top.update(top_board);
  }
  if (date_mid.readyToUpdate(millis())) {
    date_mid.update(mid_board);
  }
  delay(7);
}

