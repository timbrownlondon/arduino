#ifndef DateDisplay_h
#define DateDisplay_h

#include "Arduino.h"
#include <TM1638.h>

class CountDisplay {
  public:
    CountDisplay(TM1638* board, uint32_t start_count, uint32_t increment); 
    void update();
    void display();

  private:
    TM1638* board;
    boolean is_running;
    boolean show_LEDs;

    boolean buttonPressed();

    unsigned long interval;
    unsigned long last_update_millis;
    unsigned long last_press_millis;
    unsigned long debounce_millis;

    uint32_t count;
    uint32_t increment;
    byte last_press;
    byte last_button;
};

#endif
