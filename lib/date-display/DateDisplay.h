#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>


enum Mode {Date, Age};

class DateDisplay {
  public:
    DateDisplay(byte day, byte month, int year); 
    DateDisplay(void);
    void nextDay(void);
    void previousDay(void);
    void display(TM1638 &board);
    void setMode(TM1638 &board);
    void doAction(TM1638 &board);
    void update(int millis);

  private:
    Mode mode;
    boolean is_running;
    boolean is_forwards;

    byte day;
    byte month;
    int year;
    int birth_year;
    int interval;
    int last_update;
    byte count;
    byte last_press;
    void show_date(TM1638 &board);
    void show_year(TM1638 &board);
    void show_day(TM1638 &board);
    void show_age(TM1638 &board);
    byte lastButtonPressed(TM1638 &board);
    boolean is_leap(int year);
};

#endif
