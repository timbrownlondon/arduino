#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// refer to...
// https://arduino-info.wikispaces.com/LCD-Blue-I2C
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/
// I2C Serial Interface 16X2 Character LCD Display
// bought 3 from ebay £1.87 each, 14 April 2017

// for uno wire as
// SDA = Analog 4 (serial data)
// SCL = Analog 5 (serial clock)


// displays I have use I2C address 0x3F
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  Serial.begin(9600);  // Used to type in characters

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight

  // ------- Quick 3 blinks of backlight  -------------
  for (int i = 0; i < 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on

  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0
  lcd.setCursor(0, 0); //Start at character 4 on line 0
  lcd.print("Hello, world!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("HI!YourDuino.com");
  delay(8000);

  // Wait and then tell user they can start the Serial Monitor and type in characters to
  // Display. (Set Serial Monitor option to "No Line Ending")
  lcd.clear();
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("Use Serial Mon");
  lcd.setCursor(0, 1);
  lcd.print("Type to display");

}

void loop() {
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}
