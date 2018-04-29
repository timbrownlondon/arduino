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
  = Adafruit_NeoMatrix(8, 8, 4,
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
  matrix.Color(0, 0, 0), // off
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


#define INFRED 3       // pin connected to Passive InfraRed sensor
#define BUTTON 2
#define MAX_CREDITS 1000

#define BACKLIGHT_TIMEOUT 60000 // switch off lcd light after a minute


volatile boolean button_pressed = false;
int16_t credits = MAX_CREDITS;
int8_t pixel = 0;

volatile boolean movement_detected = false;
boolean animation_running = false;
boolean animation_growing = true;
volatile byte state = LOW;

unsigned long backlight_on_millis = 0;

void pir_on() {
  movement_detected = true;
}

void more_credits() {
  button_pressed = true;
}

void setup() {
  // set interrupt service routine for passive infra-red sensor
  pinMode(INFRED, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INFRED), pir_on, RISING);

  pinMode(BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), more_credits, RISING);

  lcd.init();
  lcd.display();
  lcd.backlight();
  show_credits(credits);

  lcd.setCursor(0, 0);
  lcd.print("READY PLAYER ONE");
  lcd.setCursor(0, 1);
  lcd.print("  credits: ");

  matrix.begin();
  matrix.setBrightness(4);

  for (int8_t i = 0; i < 8; i++) {
    matrix.fillScreen(all_colours[i]);
    matrix.show();
    delay(1000);
  }
}

void loop() {
  // add credits when button is pressed
  if (button_pressed and credits < MAX_CREDITS) {
    lcd.backlight();
    backlight_on_millis = millis();
    credits = MAX_CREDITS;
    show_credits(credits);
    button_pressed = false;
  }

  // switch lcd light if no recent activity
  if (millis() - backlight_on_millis > BACKLIGHT_TIMEOUT) {
    lcd.noBacklight();
  }

  // start a spiral animation
  if (movement_detected and ! animation_running) {
    lcd.backlight();
    backlight_on_millis = millis();

    if (credits > 0 ) {
      credits--;
    }
    show_credits(credits);

    animation_running = true;
    animation_growing = true;
    pixel = 0;

    colours[0] = all_colours[random(7)];
    colours[1] = all_colours[random(7)];
  }

  if (animation_running) {
    uint16_t colour = 0; // that means pixel off
    if (credits > 0 and animation_growing) {
      colour = colours[pixel % 2];
    }
    matrix.drawPixel(spiral[pixel][0], spiral[pixel][1], colour);
    matrix.show();

    animation_growing ? pixel++ : pixel--;

    if (pixel == 64) {
      animation_growing = false;
    }

    if (pixel < 0) {
      animation_running = false;
      movement_detected = false;
    }
  }
  delay(40);
}


void show_credits(int credits) {
  lcd.setCursor(11, 1);
  lcd.print(credits);
  lcd.print("   ");
}


