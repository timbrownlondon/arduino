#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#endif

#include <CountDisplay.h>
#include <TM1638.h>

CountDisplay::CountDisplay(TM1638* board) {
  this->board = board;
  this->mode  = 1; // seconds, minutes, hours, days, weeks, months, years
}

void CountDisplay::update_mode() {
  long next_mode = this->getButton();
  if( next_mode ){
    this->mode = next_mode;
    Serial.println(this->mode);
  }
}

// we map the buttons to the number of seconds in each period
// 60: minutes
// 3600: hours
// 86400: days, etc.
long CountDisplay::getButton() {
  switch(this->board->getButtons()){
    case 1:   return 1;
    case 2:   return 60;
    case 4:   return 3600;
    case 8:   return 86400;
    case 16:  return 604800;    // week
    case 32:  return 2629746;   // month
    case 64:  return 31556952;  // year
    case 128: return 16;
    default:  return 0;
  }
}

// provides a bit string to use the 8 led array
// as a progress bar
byte progress_bar(long numerator, long denominator){
  switch (numerator * 9 / denominator) {
    case 1: return 1;
    case 2: return 3;
    case 3: return 7;
    case 4: return 15;
    case 5: return 31;
    case 6: return 63;
    case 7: return 127;
    case 8: return 255;
    default: return 0;
  }
}

void CountDisplay::display_by_mode(unsigned long seconds){
  if ( this->mode == 1 ){
    // seconds
    this->board->setDisplayToDecNumber(seconds % 100000000, 0x00, true);
    this->board->setLEDs(1 << (seconds % 8));
    return;
  }
  if ( this->mode == 16 ){
    // seconds as hexadecimal
    this->board->setDisplayToHexNumber(seconds, 0x00, false);
    this->board->setLEDs(1 << (seconds % 8));
    return;
  }
  // cwfollowing workd for minutes, hours, days etc.
  long count = seconds / this->mode;
  long remainder = seconds - (count * this->mode);
  this->board->setDisplayToDecNumber(count, 0x00, false);
  this->board->setLEDs(progress_bar(remainder, this->mode));
  return;
}

void CountDisplay::display_seconds(unsigned long count){
  // 2022 Jan
  // ignore digits that cannot be displayed
  long n = count % 100000000;
  this->board->setDisplayToDecNumber(n, 0x48, false);
  this->board->setLEDs(1 << (n % 8));
}

void CountDisplay::display_minutes(unsigned long count){
  long n = count / 60;
  this->board->setDisplayToDecNumber(n, 0x48, false);
}

void CountDisplay::display_hours(unsigned long count){
  long n = count / 3600;
  this->board->setDisplayToDecNumber(n, 0x48, false);
}

void CountDisplay::display_days(unsigned long count){
  long n = count / 86400;
  this->board->setDisplayToDecNumber(n, 0x48, false);
}

void CountDisplay::display_months(unsigned long count){
  long n = count / 2629746;
  this->board->setDisplayToDecNumber(n, 0x48, false);
}

void CountDisplay::display_years(unsigned long count){
  long n = count / 31556952;
  this->board->setDisplayToDecNumber(n, 0x48, false);
}

void CountDisplay::display_days_decimal(unsigned long count){
  unsigned long days_times_ten_thousand = (count * 1L) / (3.6 * 2.4);
  this->board->setDisplayToDecNumber(days_times_ten_thousand, 16, false);
  this->board->setLEDs(1 << (days_times_ten_thousand % 8));
}
