// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>
#include <TimerOne.h>
#include <CountDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 board_1(8, 9, 7, true, 0);
TM1638 board_2(8, 9, 6, true, 0);

TimerOne t;

CountDisplay counter_1(&board_1, 10);
CountDisplay counter_2(&board_2, 16);

unsigned long count = 0;

void setup() {
  t.initialize(1000000);       // every second
  t.attachInterrupt(count_up); // call function on interrupt
}

void loop() {
  counter_1.display(count);
  counter_2.display(count);
}

void count_up() {
  count++;
}

