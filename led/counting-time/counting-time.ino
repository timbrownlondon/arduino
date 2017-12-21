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

void setup() {
  t.initialize(1000000);       // interrupt every second
  t.attachInterrupt(count_up); // call function on interrupt

  count = EepromAPI::readLongAt(0);
  Serial.begin(9600);
  Serial.println(count);
}

void loop() {
  counter_1.display_seconds(count);
  counter_2.display_days_hours(count);

  byte button = counter_1.getButton();

  if (button > 0) {
    save_count_to_eeprom = false;

    unsigned long n = 1;
    for (int i = button; i > 0; i--) {
      n *= 10L;
    }
    count = n - 9;
    Serial.println(button);
    Serial.println(count);
  }

  // click button 8 on lower display
  // to reset count to one million
  if (counter_2.getButton() == 8) {
    count = 1000000;
    EepromAPI::writeLongAt(0, count);
  }
}

void count_up() {
  count++;
  if (count == 99999999){
    t.stop();
  }

    // write current value of count to EEPROM when divisible by 256 (every 4mins or so)
    // the count is reset to that value after a restart
  if ((count & 0xFF) == 0 and save_count_to_eeprom) {
    EepromAPI::writeLongAt(0, count);
      Serial.print("save count: ");
      Serial.println(count);
    }
}
