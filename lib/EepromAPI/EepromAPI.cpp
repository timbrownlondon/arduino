#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <EepromAPI.h>
#include <EEPROM.h>

static void EepromAPI::writeLongAt(int address, unsigned long value) {
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

// function to return a 4 byte (32bit) long from the eeprom
// at the specified address and following three bytes
static long EepromAPI::readLongAt(int address) {
  // read four bytes from the eeprom memory
  long a = EEPROM.read(address);
  long b = EEPROM.read(address + 1);
  long c = EEPROM.read(address + 2);
  long d = EEPROM.read(address + 3);

  // recomposed thre long value by using bitshift
  return ((d << 0) & 0xFF) + ((c << 8) & 0xFFFF) + ((b << 16) & 0xFFFFFF) + ((a << 24) & 0xFFFFFFFF);
}
