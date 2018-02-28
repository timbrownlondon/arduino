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

const uint8_t spiral[][2] = {
  {3, 3}, {4, 3},
  {4, 4}, {3, 4}, {2, 4}, {2, 3},
  {2, 2}, {3, 2}, {4, 2}, {5, 2}, {5, 3}, {5, 4},
  {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {1, 4}, {1, 3}, {1, 2},
  {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
  {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1},
  {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6},
  {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}
};


#define PIR 3       // pin connected to Passive InfraRed sensor
#define BUTTON 10
#define LED 13

volatile boolean light_on = true;
volatile int money = 100;
volatile byte state = LOW;
volatile byte pixel = 0;
volatile uint16_t colour = 0;

String line1 = "";
String line2 = "";

void update_counter() {
  money--;
  pixel = 0;
  matrix.fillScreen(0);
  colour = 0;
  matrix.show();
}


void setup() {
  // set interrupt service routine for passive infra-red sensor
  pinMode(PIR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR), update_counter, RISING);

  //pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);

  matrix.begin();
  matrix.setBrightness(4);
  matrix.fillScreen(0);
  matrix.show();

  lcd.init();
  lcd.display();
  lcd.backlight();
}

void loop() {
  // toggle backlight when button is pressed
  if (digitalRead(BUTTON) and millis() - button_press_millis > DEBOUNCE_MILLIS) {
    button_press_millis = millis();
    money = 100;
    // light_on = not light_on;
    // light_on ? lcd.backlight() : lcd.noBacklight();
  }

  show_money();
  // matrix.drawPixel(random(8), random(8), colours[random(6)]);
  // matrix.drawPixel(random(8), random(8), random(65536));

  matrix.drawPixel(spiral[pixel][0], spiral[pixel][1], colour);

  if (pixel++ > 63) {
    pixel = 0;
    colour = random(65536);
  }

  matrix.show();
}


void show_money() {
  lcd.setCursor(0, 0);
  if (money < 0) {
    money = 0;
  }
  lcd.print("balance $" + String(money) + ".00  ");
  lcd.setCursor(0, 1);

  if (money < 1) {
    lcd.print("seek assistance ");
    return;
  }
  unsigned long t = millis() / 1000;
  if ( t > 3600 ) {
    t /= 60;
    lcd.print(String(t / 60) + " hr " + String(t % 60) + " min      ");
  }
  else {
    if ( t > 59 ) {
      lcd.print(String(t / 60) + " min ");
    }
    lcd.print(String(t % 60) + " sec      ");
  }
}

