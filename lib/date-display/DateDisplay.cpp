#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <DateDisplay.h>
#include <TM1638.h>

DateDisplay::DateDisplay(byte day, byte month, int year, boolean is_forwards) {
  this->day   = day;
  this->month = month;
  this->year  = year;
  this->mode = Date;
  this->is_forwards = is_forwards;

  // how often will display change (millis)
  this->interval = 256; // medium speed
  this->next_update_time = millis();
  this->count = 0;
  this->epoch = 0;
  this->a0 = analogRead(0);
  this->rand =random(0,10000);
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
  // stop, fast/medium/slow, dim/mid/bright, jump
  switch(DateDisplay::lastButtonPressed(board)) {
    case 1: this->pause(9000); break;
    case 2: this->interval = 32; break;
    case 3: this->interval = 256; break;
    case 4: this->interval = 2048; break;

    case 5: this->mode = Random; break;
    case 6: this->mode = AnalogZero; break;
    case 7: this->mode = Epoch; break;
    case 8: this->mode = Date; break;
  }
}

void DateDisplay::speed_up(){
  this->interval /= 2;
  if(this->interval < 10){
    this->interval = 4096;
  }
}

void DateDisplay::pause(int wait_millis){
  this->next_update_time = millis() + wait_millis;
  // return to Date mode after pause
  this->last_press = 8;
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
  if(millis() > this->next_update_time){
     this->count++;
     this->epoch++;
     if(!(this->count % 8)){
        this->is_forwards? DateDisplay::nextDay() : DateDisplay::previousDay();
        this->a0 = analogRead(0);
        this->rand = random(0,10000);
     }
     this->next_update_time = millis() + this->interval;
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

      if(this->year > 2038){
        this->year = 1900;
      }
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

void DateDisplay::display(TM1638 &board) {
  board.setLEDs(count ^ (count >> 1));

  if(mode == Date){ DateDisplay::show_date(board); }
  if(mode == Epoch){  DateDisplay::show_epoch(board); }
  if(mode == AnalogZero){  DateDisplay::show_analog_zero(board); }
  if(mode == Random){  DateDisplay::show_random(board); }
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


void DateDisplay::show_epoch(TM1638 &board) {
  board.setDisplayToHexNumber(this->epoch, 0, false);
}

void DateDisplay::show_random(TM1638 &board) {
  board.setDisplayToDecNumber(this->rand, 0, false);
}

void DateDisplay::show_analog_zero(TM1638 &board) {
  board.setDisplayToDecNumber(this->interval, 0, false);
}

byte binaryToGray(byte count)
{
    return count ^ (count >> 1);
}
