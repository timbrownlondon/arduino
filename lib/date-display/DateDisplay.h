#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>

class DateDisplay {
  public:
    DateDisplay(uint8_t day, uint8_t month, uint16_t year, uint16_t interval, boolean forward); 
    void nextDay(void);
    void previousDay(void);
    void update(TM1638 &board);
    boolean readyToUpdate(int millis);
  private:
    boolean is_leap(uint16_t year);
    boolean is_forward;
    int8_t day;
    int8_t month;
    uint16_t year;
    uint16_t interval;
    uint16_t last_update;
    uint8_t count;
};

#endif
