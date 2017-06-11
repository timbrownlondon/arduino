#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>


enum Mode {Date, Epoch, Random, AnalogZero};

class DateDisplay {
  public:
    DateDisplay(byte day, byte month, int year, boolean is_forwards); 
    DateDisplay(void);
    void nextDay(void);
    void previousDay(void);
    void display(TM1638 &board);
    void setMode(TM1638 &board);
    void doAction(TM1638 &board);
    void update();

  private:
    Mode mode;
    boolean is_running;
    boolean is_forwards;

    byte day;
    byte month;
    int year;
    unsigned long epoch;
    int interval;
    unsigned long next_update_time;
    byte count;
    byte binaryToGrey(byte count);
    boolean is_slow;
    byte last_press;
    void pause(int millis);
    void speed_up();
    void show_date(TM1638 &board);
    void show_year(TM1638 &board);
    void show_day(TM1638 &board);
    void show_epoch(TM1638 &board);
    void show_random(TM1638 &board);
    void show_analog_zero(TM1638 &board);
    byte lastButtonPressed(TM1638 &board);
    boolean is_leap(int year);
    int a0;
    int rand;
};

#endif
