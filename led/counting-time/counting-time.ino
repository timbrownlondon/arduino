// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>
#include <TimerOne.h>
#include <CountDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 board_1(8, 9, 7, true, 0);
TM1638 board_2(8, 9, 6, true, 0);

TimerOne t;

CountDisplay counter_1(&board_1);
CountDisplay counter_2(&board_2);

unsigned long count = (3600 * 23L) + 3590;

void setup() {
  t.initialize(1000000);       // every second
  t.attachInterrupt(count_up); // call function on interrupt
  Serial.begin(9600);
  Serial.println(count);
}

void loop() {
  counter_1.display_seconds(count);
  counter_2.display_days_hours(count);
}

void count_up() {
  count++;
}

