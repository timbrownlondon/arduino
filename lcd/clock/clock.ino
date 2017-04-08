//Sample using LiquidCrystal library
#include <LiquidCrystal.h>


// select the pins used on the LCD panel

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int offset = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
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

void setup(){
 lcd.begin(16, 2);              // start the library
}
 
void loop(){
 int n = millis()/1000 + 3585;

 lcd.setCursor(0,0);
 lcd.print(n);      // display seconds elapsed since power-up
 
 lcd.setCursor(0,7);
 lcd.print(':');

 lcd.print((round((n%3600) / 60) - round(n/60)%10) /10);

// lcd.print(round(n/600)%60);
 lcd.print(round(n/60)%10);
 
 lcd.print(':');
 lcd.print((n%60 - n%10)/10);
 lcd.print(n%10);
 
 delay(200);
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print('>');
     break;
     }
   case btnLEFT:
     {
     lcd.print('<');
     break;
     }
   case btnUP:
     {
     lcd.print('^');
     n++;
     break;
     }
   case btnDOWN:
     {
     lcd.print('v');
     n--;
     break;
     }
   case btnSELECT:
     {
     lcd.print(".");
     break;
     }
     case btnNONE:
     {
     lcd.print(' ');
     break;
     }
 }
}
