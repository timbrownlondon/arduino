#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <gamma.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// refer to...
// https://arduino-info.wikispaces.com/LCD-Blue-I2C
// https://github.com/marcoschwartz/LiquidCrystal_I2C
// NB - also checked out following
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/
// I2C Serial Interface 16X2 Character LCD Display
// bought 3 from ebay £1.87 each, 14 April 2017

// The standard pins for Uno (according to Wire.h, I guess)
// SDA = Analog 4 (serial data)
// SCL = Analog 5 (serial clock)

// contructor(I2C address, columns, rows)
// the displays I have use I2C address 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

unsigned long last_transition_millis = millis();

Adafruit_NeoMatrix matrix
  = Adafruit_NeoMatrix(8, 8, 2,
                       NEO_MATRIX_TOP  + NEO_MATRIX_RIGHT +
                       NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                       NEO_GRB         + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 0, 255),

  matrix.Color(255, 255, 0),
  matrix.Color(255, 0, 255),
  matrix.Color(0, 255, 255),

  matrix.Color(0, 0, 0)
};

// pin connected to Passive InfraRed sensor
#define PIR 3
#define BUTTON 10
int count = 0;
byte last_pir = 0;
byte pir = 0;

void setup() {
  pinMode(PIR, INPUT);
  pinMode(BUTTON, INPUT);

  matrix.begin();
  matrix.setBrightness(1);
  matrix.fillScreen(colors[2]);
  matrix.show();

  lcd.init();
}

void loop() {
  // backlight on if button pressed
  if (digitalRead(BUTTON)) {
    last_transition_millis = millis();
    lcd.setBacklight(1);
  }
  // backlight off after timeout
  if ((millis() - last_transition_millis > 10 * 1000)) {
    lcd.setBacklight(0);
  }

  // read the PIR sensor
  // 1  means movement detected
  // 0 means no movement
  pir = digitalRead(PIR);
  
  // if PIR has fired set a coloured pixel, otherwise switch one off
  uint16_t colour = (pir ?
                     (count % 2 ? colors[random(6)] : colors[count / 2 % 6] ) // sparkle or single colour
                     : colors[6] // pixel off
                    );

  matrix.drawPixel(random(8), random(8), colour);
  matrix.show();


  if (pir != last_pir) {
    last_pir = pir;
    last_transition_millis = millis();

    if (pir) {
      lcd.setCursor(0, 1);
      lcd.print(++count);
    }
  }



  delay(7);
}


