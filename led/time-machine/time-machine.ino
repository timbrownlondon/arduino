// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>

#include <DateDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 mid_board(8, 9, 6, true, 0);
TM1638 top_board(8, 9, 7, true, 0);

DateDisplay date_top(4, 4, 1962);
DateDisplay date_mid(6, 3, 1999);

void setup() {
}

void loop() {
  date_top.doAction(top_board);
  date_top.update(millis());
  date_top.display(top_board);

  date_mid.doAction(mid_board);
  date_mid.update(millis());
  date_mid.display(mid_board);
}

