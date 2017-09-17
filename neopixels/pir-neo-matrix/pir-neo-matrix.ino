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

unsigned long button_press_millis = millis();
unsigned long last_move_millis = millis();
boolean display_is_on = true;
#define DEBOUNCE_MILLIS 200

Adafruit_NeoMatrix matrix
  = Adafruit_NeoMatrix(8, 8, 2,
                       NEO_MATRIX_TOP  + NEO_MATRIX_RIGHT +
                       NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                       NEO_GRB         + NEO_KHZ800);

const uint16_t colours[] = {
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
int count = 1;
byte last_pir = 0;
byte pir = 0;

void setup() {
  pinMode(PIR, INPUT);
  pinMode(BUTTON, INPUT);

  matrix.begin();
  matrix.setBrightness(2);
  matrix.fillScreen(colours[1]);
  matrix.show();

  lcd.init();
  lcd.display();
  lcd.setBacklight(1);
}

void loop() {

  // toggle backlight when button is pressed`
  if (digitalRead(BUTTON) and millis() - button_press_millis > DEBOUNCE_MILLIS) {
    button_press_millis = millis();
    display_is_on = not display_is_on;
    lcd.setBacklight(display_is_on);
    if (display_is_on) {
      lcd.display();
    }
    else {
      lcd.noDisplay();
    }
  }

  // read the PIR sensor
  // 1  means movement detected
  // 0 means no movement
  pir = digitalRead(PIR);

  if (pir != last_pir) {
    last_pir = pir;
    if (pir) {
      last_move_millis = millis();
      show_message(++count);
    }
  }


  // if recent movement we set some colour else we switch one pixel off (colour 6)
  if (millis() - last_move_millis < 30 * 1000) {
    int i = random(5) < 4 ?
            count % 6 : random(6);

    matrix.drawPixel(random(8), random(8), colours[i]);
  }
  else {
    matrix.drawPixel(random(8), random(8), colours[6]);
  }

  matrix.show();
  delay(10);
}

String subjects[6] = {"I", "You", "They", "He", "She", "We"};
String verbs[4]    = {" watched ", " looked at ", " glanced at ", " saw "};
String objects[6]  = {"me ", "you ", "them ", "him ", "her ", "us "};


void show_message(int i) {
  // make a message with some randomness

  String subj = subjects[random(6)];
  String  obj =  objects[random(6)];


  lcd.setCursor(0, 0);
  lcd.print(subjects[random(6)] + verbs[random(4)] +  "          ");
  lcd.setCursor(0, 1);
  lcd.print(objects[random(6)] + String(i) + " times.       ");

}

