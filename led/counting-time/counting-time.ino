// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>
#include <TimerOne.h>
#include <EepromAPI.h>
#include <CountDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 board_1(8, 9, 7, true, 0);
TM1638 board_2(8, 9, 6, true, 0);

TimerOne t;

CountDisplay counter_1(&board_1);
CountDisplay counter_2(&board_2);

unsigned long count;
boolean save_count_to_eeprom = true;

#define BUZZER 10
boolean trigger_buzzer = false;

void setup() {
  t.initialize(1000000);       // interrupt every second
  t.attachInterrupt(count_up); // call function on interrupt

  pinMode(BUZZER, OUTPUT);

  count = EepromAPI::readLongAt(0);
  Serial.begin(9600);
  Serial.println(count);
}

void loop() {
  counter_1.display_seconds(count);
  counter_2.display_days_hours(count);

  if (counter_1.getButton() == 1) {
    count = 12339980;
  }
}

void count_up() {
  count++;
  
  /*
    if (count == 99999999) {
      t.stop();
    }
  */

  // sound pips from ...9991 to ...0000
  if (count % 10000 > 9990) {
    tone(BUZZER, 262, 50);
  }
  else if (count % 10000 == 0) {
    tone(BUZZER, 262, 500);
  }

  // write current value of count to EEPROM when divisible by 256 (every 4mins or so)
  // the count is reset to that value after a restart
  if ((count & 0xFF) == 0 and save_count_to_eeprom) {
    EepromAPI::writeLongAt(0, count);
  }
}
