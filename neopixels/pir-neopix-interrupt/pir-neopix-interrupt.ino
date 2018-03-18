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

// this runs the 8x8 Pimoroni NeoPixel matrix
Adafruit_NeoMatrix matrix
  = Adafruit_NeoMatrix(8, 8, 2,
                       NEO_MATRIX_TOP  + NEO_MATRIX_RIGHT +
                       NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                       NEO_GRB         + NEO_KHZ800);

const uint16_t all_colours[] = {
  matrix.Color(255, 0, 0), // red
  matrix.Color(0, 255, 0), // green
  matrix.Color(0, 0, 255), // blue

  matrix.Color(255, 255, 0), // yellow
  matrix.Color(255, 0, 255), // pink
  matrix.Color(0, 255, 255), // cyan

  matrix.Color(255, 127, 0), // orange

};

uint16_t colours[2];

// co-ords of 8x8 matrix ordered as clockwise spiral
const uint8_t spiral[][2] = {
  {3, 3}, {4, 3},
  {4, 4}, {3, 4}, {2, 4}, {2, 3},
  {2, 2}, {3, 2}, {4, 2}, {5, 2}, {5, 3}, {5, 4},
  {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {1, 4}, {1, 3}, {1, 2},
  {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
  {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1},
  {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6},
  {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}, {0, 7}
};


#define PIR 3       // pin connected to Passive InfraRed sensor
#define BUTTON 10
#define MIN_CASH -5


int8_t money = 5;
int8_t pixel = 0;

volatile boolean movement_detected = false;
boolean animation_running = false;
boolean animation_growing = true;

void pir_on() {
  movement_detected = true;
}


void setup() {
  // set interrupt service routine for passive infra-red sensor
  pinMode(PIR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR), pir_on, RISING);

  pinMode(BUTTON, INPUT);

  matrix.begin();
  matrix.setBrightness(4);

  for (int8_t i = 0; i < 7; i++) {
    matrix.fillScreen(all_colours[i]);
    matrix.show();
    delay(2000);
  }

  matrix.fillScreen(0);
  matrix.show();

  lcd.init();
  lcd.display();
  lcd.noBacklight();
  show_money(money);
}

void loop() {
  // add money when button is pressed
  if (digitalRead(BUTTON) and money < 120) {
    lcd.backlight();
    show_money(++money);
  }

  // start a spiral animation
  if (movement_detected and ! animation_running) {
    animation_running = true;
    animation_growing = true;
    lcd.backlight();
    pixel = 0;

    colours[0] = all_colours[random(7)];
    colours[1] = all_colours[random(7)];
  }

  if (animation_running) {
    uint16_t colour = 0; // that means pixel off
    if (money > MIN_CASH and animation_growing) {
      colour = colours[pixel % 2];
    }
    matrix.drawPixel(spiral[pixel][0], spiral[pixel][1], colour);

    matrix.show();

    animation_growing ? pixel++ : pixel--;

    if (pixel == 64) {
      animation_growing = false;

      if (money > MIN_CASH ) {
        money--;
      }
      show_money(money);
    }

    if (pixel < 0) {
      animation_running = false;
      movement_detected = false;
      lcd.noBacklight();
    }
  }
  delay(40);
}


void show_money(int money) {
  lcd.setCursor(0, 0);
  lcd.print("Balance ");
  if (money < 0) {
    lcd.print("-");
  }
  lcd.print("$" + String(abs(money)) + ".00  ");

  lcd.setCursor(0, 1);
  money < 1 ?
  lcd.print("Seek Assistance ") :
  lcd.print("                ");
}


