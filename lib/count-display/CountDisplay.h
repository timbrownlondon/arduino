#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>


class CountDisplay {
  public:
    CountDisplay(TM1638* board, byte number_base);
    void display(unsigned long);
    void display();

  private:
    TM1638* board;
    boolean is_running;
    boolean show_LEDs;

    boolean buttonPressed();
    byte getLastButtonPress();
    byte whichButtonPressed();

    unsigned long interval;
    unsigned long last_update_millis;
    unsigned long last_press_millis;
    unsigned long debounce_millis;
    unsigned long count;

    byte last_button_pressed;
    byte number_base;

};

#endif
