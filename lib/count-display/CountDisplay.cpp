#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <CountDisplay.h>
#include <TM1638.h>

#define MAX_INTERVAL   1000
#define MIN_INTERVAL    125
#define DEBOUNCE_MILLIS 250

CountDisplay::CountDisplay(TM1638* board) {
  this->board = board;
  this->show_LEDs = true;
  this->days  = -1;
  this->hours = -1;
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


void CountDisplay::display_seconds(unsigned long count){
  this->board->setDisplayToDecNumber(count, 0, false);

  this->show_LEDs?
    this->board->setLEDs(1 << (count - 1) % 8):
    this->board->setLEDs(0);
}

void CountDisplay::display_days_hours(unsigned long count){

  int hours = (count/3600L) % 24;

  if(hours != this->hours){

    hours / 10 > 0?
      this->board->setDisplayDigit(hours / 10, 5, false):
      this->board->clearDisplayDigit(5, false);

    this->board->setDisplayDigit(hours % 10, 6, false);
    this->board->setDisplayToString("h", 0, 7);
    this->hours = hours;
  }

  int days = count/(3600 * 24L);
  if(days != this->days){
    
    days / 100 > 0?
      this->board->setDisplayDigit(days / 100, 0, false):
      this->board->clearDisplayDigit(0, false);

    days / 10 > 0?
      this->board->setDisplayDigit(days / 10, 1, false):
      this->board->clearDisplayDigit(1, false);

    this->board->setDisplayDigit(days % 10, 2, false);
    this->board->setDisplayToString("d", 0, 3);
    this->days = days;
  }

  // show hour progress as incrementing LEDs
  byte hour_part = (count % 3600L) / 400;
  this->board->setLEDs((1 << hour_part) - 1);
}

