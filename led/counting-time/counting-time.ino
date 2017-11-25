// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>

#include <CountDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 board_1(8, 9, 7, true, 0);
TM1638 board_2(8, 9, 6, true, 0);

CountDisplay counter_1(&board_1, 0, 1);
CountDisplay counter_2(&board_2, 99999999, -1);

void setup() {
  counter_1.display();
  counter_2.display();
}

void loop() {
  counter_1.update();
  counter_2.update();
}

