#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <DateDisplay.h>
#include <TM1638.h>

DateDisplay::DateDisplay(uint8_t d, uint8_t m, uint16_t y, uint16_t interval, boolean is_forward) {
  day   = d;
  month = m;
  year  = y;
  // how often will display change (millis)
  this->interval    = interval;
  last_update = millis();
  count = 0;
  this->is_forward = is_forward;
}

// this array starts Dec, Jan, Feb...
// month[0] is 31 (December) 
uint8_t month_length[13] = {31, 31,28,31, 30,31,30, 31,31,30, 31,30,31};

boolean DateDisplay::is_leap(uint16_t year) {
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

boolean DateDisplay::readyToUpdate(int millis){
  if(millis - last_update > interval){
     count++;
     if(! count) { //every 256th cycle
       if(is_forward){
         nextDay();
       }
       else {
         previousDay();
       }
     }
     last_update = millis;
     return true;
  }
  return false;
}

void DateDisplay::nextDay() {
  day++;
  if(day > month_length[month]){
    day = 1;
    month++;

    if(month > 12){
      month = 1;
      year++;

      if(year > 2100){
        year = 1800;
      }
    }
  }
}

void DateDisplay::previousDay() {
  day--;
  if(day < 1){
    month--;
    day = month_length[month];

    if(month < 1){
      month = 12;
      year--;

      if(year < 1800){
        year = 2100;
      }
    }
  }
}

void DateDisplay::update(TM1638 &board) {
  // LEDs
  board.setLEDs(count);

  // days...
  if(day / 10){
    board.setDisplayDigit(day / 10, 0, false);
  }
  else {
    board.clearDisplayDigit(0, false);
  }
  board.setDisplayDigit(day % 10, 1, true);

  // months...
  if(month / 10){
    board.setDisplayDigit(month / 10, 2, false);
  }
  else {
    board.clearDisplayDigit(2, false);
  }
  board.setDisplayDigit(month % 10, 3, true);

  // year...
  board.setDisplayDigit(year/1000, 4, false);
  board.setDisplayDigit((year/100) % 10, 5, false);
  board.setDisplayDigit((year % 100)/10, 6, false);
  board.setDisplayDigit(year % 10, 7, false);
}

