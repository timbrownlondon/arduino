// https://github.com/rjbatista/tm1638-library
#include <TM1638.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <CountDisplay.h>

// data pin 8, clock pin 9, strobe pin 6/7, activate, brightness 0-7
TM1638 board_1(8, 9, 7, true, 0);
TM1638 board_2(8, 9, 6, true, 0);

TimerOne t;

CountDisplay counter_1(&board_1);
CountDisplay counter_2(&board_2);

// we write current value of count to eeprom memory
// every UPDATE_INTERVAL seconds
#define UPDATE_INTERVAL 60 * 15
unsigned long count;

void setup() {
  t.initialize(1000000);       // interrupt every second
  t.attachInterrupt(count_up); // call function on interrupt

  count = readLongAt(0);
  Serial.begin(9600);
  Serial.print("count: ");
  Serial.println(count);
  Serial.print("update: ");
  Serial.println(UPDATE_INTERVAL);
}

void loop() {
  counter_1.display_seconds(count);
  counter_2.display_days_hours(count);
}

void count_up() {
  count++;
  // write current value of count to EEPROM
  if (count % UPDATE_INTERVAL == 0) {
    writeLongAt(0, count);
    Serial.println(count);
  }
}

void writeLongAt(int address, unsigned long value) {
  //Decomposition from an unsigned long to 4 bytes by using bitshift.
  byte d = ((value >>  0) & 0xFF);
  byte c = ((value >>  8) & 0xFF);
  byte b = ((value >> 16) & 0xFF);
  byte a = ((value >> 24) & 0xFF);

  // write four bytes to the eeprom memory
  EEPROM.write(address + 0, a);
  EEPROM.write(address + 1, b);
  EEPROM.write(address + 2, c);
  EEPROM.write(address + 3, d);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long readLongAt(int address) {
  // read four bytes from the eeprom memory
  long a = EEPROM.read(address);
  long b = EEPROM.read(address + 1);
  long c = EEPROM.read(address + 2);
  long d = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((d << 0) & 0xFF) + ((c << 8) & 0xFFFF) + ((b << 16) & 0xFFFFFF) + ((a << 24) & 0xFFFFFFFF);
}
