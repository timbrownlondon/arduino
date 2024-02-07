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

    void display_minutes(unsigned long);
    void display_hours(unsigned long);
    void display_days(unsigned long);
    void display_months(unsigned long);
    void display_years(unsigned long);
    long getButton();

    void display_by_mode(unsigned long);
    void update_mode();

  private:
    TM1638* board;
    boolean show_LEDs;
    long mode;

};

#endif
