#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <DateDisplay.h>
#include <TM1638.h>

// update speed can be modified
// interval is the delay between each update
#define MAX_INTERVAL 10000
#define MIN_INTERVAL 10

DateDisplay::DateDisplay(TM1638* board, byte day, byte month, int year, boolean is_forwards) {
  this->board = board;
  this->day   = day;
  this->month = month;
  this->year  = year;
  this->is_forwards = is_forwards;

  // how often will display change (millis)
  this->interval = 160; // fast update
  this->last_update_millis = millis();
  this->count = 0;
  this->is_running = true;
  this->show_LEDs = true;
  this->debounce_millis = 200;
}


void DateDisplay::processButtons() {
  if(millis() - this->last_press_millis > this->debounce_millis) {
    this->last_press_millis = millis();

    switch(this->board->getButtons()) {
      case 1: // button 1: stop/go
		   	  this->is_running = !this->is_running;
			  break;
	  case 2: // button 2
    	this->is_running = true;
        this->interval /= 2;
        if(this->interval < MIN_INTERVAL){
          this->interval = MIN_INTERVAL;
        }
		break;
      case 4: // button 3
        this->is_running = true;
        this->interval *= 2;
        if(this->interval > MAX_INTERVAL){
          this->interval = MAX_INTERVAL;
		}
		break;
      case 8: // button 4
        this->show_LEDs = !this->show_LEDs;
        if(!this->show_LEDs){
          this->board->setLEDs(0);
        }
        break;
//      case 16:  this.brighter();
//      case 32:  this.randomDay();
//      case 64:  this.randomMonth();
//      case 128: this.randomYear();
	}
  }
}


// this array starts Dec, Jan, Feb...
// month[0] is 31 (December) 
uint8_t month_length[13] = {31, 31,28,31, 30,31,30, 31,31,30, 31,30,31};

boolean DateDisplay::is_leap(int year) {
  if(year % 400 == 0){
    return true;
  }
  if(year % 100 == 0){
    return false;
  }
  if(year % 4 == 0){
    return true;
  }
  return false; 
}

void DateDisplay::update(){
  if(this->is_running and millis() > this->last_update_millis + this->interval){
     this->is_forwards? this->count++ : this->count--;
     if(!(this->count % 8)){
        this->is_forwards? DateDisplay::nextDay() : DateDisplay::previousDay();
     }
     this->last_update_millis = millis() + this->interval;
  }
}

void DateDisplay::nextDay() {
  this->day++;
  if(this->day > month_length[month]){
    this->day = 1;
    this->month++;

    if(this->month > 12){
      this->month = 1;
      this->year++;

      this->year %= 100;
    }
  }
  return true;
}

void DateDisplay::previousDay() {
  this->day--;
  if(this->day < 1){
    this->month--;
    this->day = month_length[month];

    if(this->month < 1){
      this->month = 12;
      this->year--;

      if(this->year < 1800){
        this->year = 2100;
      }
    }
  }
}

void DateDisplay::display() {
  if(this->show_LEDs){
    board->setLEDs(1 << (count % 8));
  }
  this->show_ddmmyy();
}

void DateDisplay::show_ddmmyy() {
  this->board->setDisplayDigit(this->day / 10, 0, false);
  this->board->setDisplayDigit(this->day % 10, 1, false);
  this->board->setDisplayToString("-", 0, 2); // display hyphen
  this->board->setDisplayDigit(this->month / 10, 3, false);
  this->board->setDisplayDigit(this->month % 10, 4, false);
  this->board->setDisplayToString("-", 0, 5); // display hyphen
  this->board->setDisplayDigit(this->year % 100 / 10, 6, false);
  this->board->setDisplayDigit(this->year % 10, 7, false);
}


void DateDisplay::show_date() {
  // days...
  if(this->day / 10){
    board->setDisplayDigit(this->day / 10, 0, false);
  }
  else {
    board->clearDisplayDigit(0, false);
  }
  board->setDisplayDigit(this->day % 10, 1, true);

  // months...
  if(this->month / 10){
    board->setDisplayDigit(this->month / 10, 2, false);
  }
  else {
    board->clearDisplayDigit(2, false);
  }
  board->setDisplayDigit(this->month % 10, 3, true);

  // year...
  board->setDisplayDigit(this->year/1000, 4, false);
  board->setDisplayDigit((this->year/100) % 10, 5, false);
  board->setDisplayDigit((this->year % 100)/10, 6, false);
  board->setDisplayDigit(this->year % 10, 7, false);
}

