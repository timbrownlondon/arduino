#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  Serial.println(EEPROM.length());

  unsigned long count = readLongAt(0);

  Serial.println(count);

  Serial.println(EEPROM.read(0));
  Serial.println(EEPROM.read(1));
  Serial.println(EEPROM.read(2));
  Serial.println(EEPROM.read(3));

  //count = 60L * 60 * 24 * 999;
  writeLongAt(0, 123456789);
}

void loop() {}

//This function writes a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void writeLongAt(int address, unsigned long value) {
  //Decomposition from an unsigned long to 4 bytes by using bitshift.
  byte d = ((value >>  0) & 0xFF);
  byte c = ((value >>  8) & 0xFF);
  byte b = ((value >> 16) & 0xFF);
  byte a = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address + 0, a);
  EEPROM.write(address + 1, b);
  EEPROM.write(address + 2, c);
  EEPROM.write(address + 3, d);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long readLongAt(int address){
  //Read the 4 bytes from the eeprom memory.
  long a = EEPROM.read(address);
  long b = EEPROM.read(address + 1);
  long c = EEPROM.read(address + 2);
  long d = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((d << 0) & 0xFF) + ((c << 8) & 0xFFFF) + ((b << 16) & 0xFFFFFF) + ((a << 24) & 0xFFFFFFFF);
}


