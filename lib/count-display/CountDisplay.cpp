#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <CountDisplay.h>
#include <TM1638.h>

CountDisplay::CountDisplay(TM1638* board) {
  this->board = board;
  this->hours = -1;
  this->days  = -1;
}


byte CountDisplay::getButton() {

  switch(this->board->getButtons()){
    case 1:   return 1;
    case 2:   return 2;
    case 4:   return 3;
    case 8:   return 4;
    case 16:  return 5;
    case 32:  return 6;
    case 64:  return 7;
    case 128: return 8;
    default:  return 0;
  }
}

void CountDisplay::display_seconds(unsigned long count){
  this->board->setDisplayToDecNumber(count, 0, false);
  this->board->setLEDs(1 << (count - 1) % 8);
}

void CountDisplay::display_days_decimal(unsigned long count){
  unsigned long days = (count * 1000L) / (36 * 24);
  this->board->setDisplayToDecNumber(days, 32, false);
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

