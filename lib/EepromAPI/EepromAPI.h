#ifndef EepromAPI_h
#define EepromAPI_h

class EepromAPI {
  public:
    static void EepromAPI::writeLongAt(int address, unsigned long value);

    static long EepromAPI::readLongAt(int address);
};

#endif
