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

CountDisplay::CountDisplay(TM1638* board, uint32_t start, uint32_t i) {
  this->board = board;
  this->count = start;
  this->increment = i;

  // how often will display change (millis)
  this->interval = MAX_INTERVAL; 
  this->last_update_millis = millis() + 5000; // leave 5 seconds before counting
  this->is_running = true;
  this->show_LEDs = true;
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


void CountDisplay::update(){
  if(this->buttonPressed()){
    this->display();
  }

  if(this->is_running and millis() > this->last_update_millis + this->interval){
    this->last_update_millis = millis(); 
    this->count += this->increment;
    this->display();
  }
}

void CountDisplay::display(){
  this->board->setDisplayToDecNumber(this->count, 0, false);
  this->show_LEDs?
    this->board->setLEDs(1 << (count % 8)):
    this->board->setLEDs(0);
}
