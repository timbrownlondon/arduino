#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>

class DateDisplay {
  public:
    DateDisplay(TM1638* board, byte day, byte month, int year, boolean is_forwards); 
	void processButtons();
    void update();
    void display();

  private:
	TM1638* board;
    boolean is_running;
    boolean is_forwards;
    boolean show_LEDs;

    byte day;
    byte month;
    int year;

    unsigned long interval;
    unsigned long last_update_millis;
	unsigned long last_press_millis;
	unsigned long debounce_millis;

    byte count;
    byte last_press;
	byte last_button;

    void nextDay();
    void previousDay();
    void show_ddmmyy();
    void show_date();

    boolean is_leap(int year);
};

#endif
