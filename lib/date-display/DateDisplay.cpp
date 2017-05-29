#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <DateDisplay.h>
#include <TM1638.h>

DateDisplay::DateDisplay(byte day, byte month, int year) {
  this->day   = day;
  this->month = month;
  this->year  = this->birth_year  = year;
  this->mode = Date;

  // how often will display change (millis)
  this->interval    = 100;
  this->last_update = millis();
  this->count = 0;
  this->is_running = true;
}

byte DateDisplay::lastButtonPressed(TM1638 &board) {
  switch(board.getButtons()) {
    case 1:   this->last_press = 1; break;
    case 2:   this->last_press = 2; break;
    case 4:   this->last_press = 3; break;
    case 8:   this->last_press = 4; break;
    case 16:  this->last_press = 5; break;
    case 32:  this->last_press = 6; break;
    case 64:  this->last_press = 7; break;
    case 128: this->last_press = 8; break;
  }
  return this->last_press;
}

void DateDisplay::doAction(TM1638 &board) {
  // start/stop, slow/mid/fast, dim/mid/bright, jump
  switch(DateDisplay::lastButtonPressed(board)) {
    case 1: this->is_running = false; break;
    case 2: this->is_running = true; break;
    case 7: this->mode = Age; break;
    case 8: this->mode = Date; break;
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

void DateDisplay::update(int millis){
  if(! this->is_running){
    return;
  }
  if(millis - last_update > interval){
     this->count++;
     if(! this->count) { //every 256th cycle
       DateDisplay::nextDay();
     }
     this->last_update = millis;
     return true;
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

      if(this->year > 2029){
        this->year = 1950;
      }
    }
  }
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

void DateDisplay::display(TM1638 &board) {
  board.setLEDs(count);

  if(mode == Date){ DateDisplay::show_date(board); }
  if(mode == Age){  DateDisplay::show_age(board); }
}

void DateDisplay::show_date(TM1638 &board) {
  // days...
  if(this->day / 10){
    board.setDisplayDigit(this->day / 10, 0, false);
  }
  else {
    board.clearDisplayDigit(0, false);
  }
  board.setDisplayDigit(this->day % 10, 1, true);

  // months...
  if(this->month / 10){
    board.setDisplayDigit(this->month / 10, 2, false);
  }
  else {
    board.clearDisplayDigit(2, false);
  }
  board.setDisplayDigit(this->month % 10, 3, true);

  // year...
  board.setDisplayDigit(this->year/1000, 4, false);
  board.setDisplayDigit((this->year/100) % 10, 5, false);
  board.setDisplayDigit((this->year % 100)/10, 6, false);
  board.setDisplayDigit(this->year % 10, 7, false);
}


void DateDisplay::show_age(TM1638 &board) {
  board.setDisplayToString("YEARS", 0, 3);

  int age = this->year - this->birth_year;
  if(age / 10){
    board.setDisplayDigit(age / 10, 0, false);
  }
  else {
    board.clearDisplayDigit(0, false);
  }
  board.setDisplayDigit(age % 10, 1,false);
}

