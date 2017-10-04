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
boolean display_message = true;
const String blank = "                ";

const String padding[] = {"", " ", "  ", "   ", "    ", "     ", "      ", "       ", "        "};


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
byte last_pir = 0;
byte pir = 0;
unsigned long count = 0;
String line1 = "";
String line2 = "";

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

  // toggle backlight when button is pressed
  // also toggle message mode
  if (digitalRead(BUTTON) and millis() - button_press_millis > DEBOUNCE_MILLIS) {
    button_press_millis = millis();
    display_message = not display_message;
  }

  display_message ? show_message() : show_count(count);

  // read the PIR sensor
  // 1  means movement detected
  // 0 means no movement
  pir = digitalRead(PIR);
  if (pir != last_pir) {
    last_pir = pir;
    if (pir) {
      last_move_millis = millis();
      count++;

      String msg = message();
      if (count % 2 == 1) {
        line1 = msg + padding[16 - msg.length()];
        line2 = blank;
      }
      else {
        line1 = blank;
        line2 = padding[16 - msg.length()] + msg;
      }
    }
  }

  // if recent movement we set some colour else we switch one pixel off (colour 6)
  if (millis() - last_move_millis < 30 * 1000) {
    int i = random(5) < 4 ? count % 6 : random(6);
    matrix.drawPixel(random(8), random(8), colours[i]);
  }
  else {
    matrix.drawPixel(random(8), random(8), colours[6]);
  }
  matrix.show();
}


const String colour_names[6] = {"red", "green", "blue", "yellow", "pink", "cyan"};
void show_count(int i) {
  lcd.setCursor(0, 0);
  lcd.print(String(i) + " " + colour_names[i % 6] + blank);
  lcd.setCursor(0, 1);
  unsigned long t = millis() / 1000;
  if ( t > 3600 ) {
    t /= 60;
    lcd.print(String(t / 60) + " hr " + String(t % 60) + " min" + blank);
  }
  else {
    if ( t > 59 ) {
      lcd.print(String(t / 60) + " min ");
    }
    lcd.print(String(t % 60) + " sec" + blank);
  }
}

void show_message() {
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

const String subjects[6] = {"I ", "you ", "they ", "he ", "she ", "we "};
const String verbs[8]    = {"watched", "looked at", "glanced at", "saw", "will see", "never saw", "will watch", "didn't see"};
const String objects[6]  = {" me", " you", " them", " him", " her", " us"};

String message() {
  while (true) {
    String line = subjects[random(6)] + verbs[random(8)] + objects[random(6)];
    if (line.length() < 17) {
      return line;
    }
  }
}

