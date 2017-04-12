//Sample using LiquidCrystal library
#include <LiquidCrystal.h>


// select the pins used on the LCD panel

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int offset      = 0;
int hrs = 0;
int mins = 0;
int secs = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons() {
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}


int length_of_int(int n) {
  if (n < 10) {
    return 1;
  }
  if (n < 100) {
    return 2;
  }
  if (n < 1000) {
    return 3;
  }
  if (n < 10000) {
    return 4;
  }
  if (n < 100000) {
    return 5;
  }
  return 6;
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("run time");
}

void loop() {
  int secs = millis() / 1000;

  lcd.setCursor(16 - length_of_int(secs), 0);
  lcd.print(secs);

  lcd.setCursor(2, 1);
  hrs = round(secs / 3600);
  lcd.print(hrs);
  lcd.print("h ");

  mins = round((secs - 3600 * hrs) / 60);

  lcd.print(mins);
  lcd.print("m ");

  lcd.print(secs - (60 * mins + 3600 * hrs));
  lcd.print("s ");

  delay(200);
  lcd_key = read_LCD_buttons();  // read the buttons

  switch (lcd_key) {              // depending on which button was pushed, we perform an action
    case btnRIGHT: {
        lcd.print('>');
        break;
      }
    case btnLEFT: {
        lcd.print('<');
        break;
      }
    case btnUP: {
        lcd.print('^');
        break;
      }
    case btnDOWN: {
        lcd.print('v');
        break;
      }
    case btnSELECT: {
        lcd.print(".");
        break;
      }
    case btnNONE: {
        lcd.print(' ');
        break;
      }
  }
}
