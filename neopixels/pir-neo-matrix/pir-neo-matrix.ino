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



Adafruit_NeoMatrix matrix
  = Adafruit_NeoMatrix(8, 8, 2,
                       NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
                       NEO_MATRIX_ROWS   + NEO_MATRIX_ZIGZAG,
                       NEO_GRB           + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 0, 255),

  matrix.Color(255, 255, 0),
  matrix.Color(255, 0, 255),
  matrix.Color(0, 255, 255),

  matrix.Color(255, 255, 255)
};

#define PIR 3
int count = 0;
byte last_pir = 0;
byte this_pir = 0;

void setup() {
  pinMode(PIR, INPUT);

  matrix.begin();
  matrix.setBrightness(1);
  matrix.fillScreen(colors[2]);
  matrix.show();

  lcd.init();
  lcd.backlight();
}


void loop() {
  for (int i = 0; i < 7; i++) {
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        matrix.drawPixel(x, y, colors[(i + x + y) % 7]);
        matrix.show();
        delay(20);

        // read the PIR sensor 
        // 1  means movement detected
        // 0 means no movement
        this_pir = digitalRead(PIR);
        if (this_pir != last_pir) {
          last_pir = this_pir;
          count += this_pir;
          lcd.setCursor(0, 1);
          lcd.print(count);
        }
      }
    }
  }
}
