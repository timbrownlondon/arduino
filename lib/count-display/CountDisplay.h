#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>

class CountDisplay {
  public:
    CountDisplay(TM1638* board);
    void display_seconds(unsigned long);
    void display_days_hours(unsigned long);
    void display_days_decimal(unsigned long);
    byte getButton();

  private:
    TM1638* board;
    boolean show_LEDs;
    int hours;
    int days;

};

#endif
