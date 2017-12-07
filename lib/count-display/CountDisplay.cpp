#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <CountDisplay.h>
#include <TM1638.h>

// update speed can be modified
// interval is the delay between each update
#define MAX_INTERVAL   1000
#define MIN_INTERVAL    125
#define DEBOUNCE_MILLIS 250

CountDisplay::CountDisplay(TM1638* board, byte number_base) {
  this->board = board;
  this->show_LEDs = true;
  this->count = 0;
  this->number_base = number_base;
}


boolean CountDisplay::buttonPressed() {
  if(millis() - this->last_press_millis > DEBOUNCE_MILLIS) {
    this->last_press_millis = millis();

    switch(this->board->getButtons()) {
      case 1: // button 1: stop/go
        this->is_running = !this->is_running;
        return true;
      case 2: // button 2
    	this->is_running = true;
        this->interval /= 2;
        if(this->interval < MIN_INTERVAL){
          this->interval = MIN_INTERVAL;
        }
        return true;
      case 4: // button 3
        this->is_running = true;
        this->interval *= 2;
        if(this->interval > MAX_INTERVAL){
          this->interval = MAX_INTERVAL;
	}
        return true;
      case 8: // button 4
        this->show_LEDs = !this->show_LEDs;
        return true;
//      case 16:  this.brighter();
//      case 32:  this.randomDay();
//      case 64:  this.randomMonth();
//      case 128: this.randomYear();
    }
  }
  return false;
}

byte CountDisplay::getLastButtonPress() {
  if(millis() - this->last_press_millis > DEBOUNCE_MILLIS) {
    this->last_press_millis = millis();

    byte i = this->board->getButtons();

    if(i > 0){
      this->last_button_pressed = i;
    }
  }
  return this->last_button_pressed;
}

byte CountDisplay::whichButtonPressed(){
  return this->last_button_pressed;
}


void CountDisplay::display(unsigned long count){
  this->number_base == 10?
    this->board->setDisplayToDecNumber(count, 0, false):
    this->board->setDisplayToHexNumber(count, 0, false);

  this->show_LEDs?
    this->board->setLEDs(1 << (count % 8)):
    this->board->setLEDs(0);
}
