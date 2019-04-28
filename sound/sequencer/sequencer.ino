// lib for LED & KEY board
// constructor params: data DIO, clock CLK, strobe STB, activate, brightness 0-7
#include <TM1638.h>
TM1638 led(7, 6, 5, true, 0);

#include <TimerOne.h>
TimerOne t;

#include "notes.h"

// pins for rotary encoder
#define ROTATE_A    2
#define ROTATE_B    3
#define ROTATE_PUSH 4

boolean stopped = false;
unsigned long lastPushMillis = 0;
#define DEBOUNCE_PUSH_MILLIS 500

// first eight slots are notes
// then speed and duration
unsigned int slot[10]   = {1, 2, 3, 4, 5, 6, 7, 8, 15, 30};
volatile byte slotIndex = 0;

unsigned long speeds[20] = {
  2219000,
  1849000,
  1540000,
  1284000,
  1070000,
  892000,
  743000,
  619000,
  516000,
  430000,
  358000,
  299000,
  249000,
  207000,
  173000,
  144000,
  120000,
  100000,
  83300,
  69400,
};
int maxSpeedIndex = sizeof(speeds) / sizeof(*speeds) - 1;


volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating = false;    // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;


void setup() {
  // default interrupt every second
  t.initialize(speeds[slot[8]]);
  t.attachInterrupt(nextSlot);

  pinMode(ROTATE_A, INPUT);
  pinMode(ROTATE_B, INPUT);
  pinMode(ROTATE_PUSH, INPUT);
  // turn on pullup resistors
  digitalWrite(ROTATE_A, HIGH);
  digitalWrite(ROTATE_B, HIGH);
  digitalWrite(ROTATE_PUSH, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin(9600);  // output
}

void loop() {
  rotating = true;
  byte pressed = getButton(&led);

  if (pressed > 7) { // greater than seven is no press
    //Serial.println(slotIndex);
    led.setLEDs(1 << slotIndex);
    led.setDisplayToSignedDecNumber(slot[slotIndex], 0, false);
    if (stopped)
      updateDuration();
    else
      updateSpeed();
  }
  else {
    slotUpdated(pressed);
    led.setLEDs(1 << pressed);
    led.setDisplayToSignedDecNumber(slot[pressed], 0, false);
  }

  if (digitalRead(ROTATE_PUSH) == LOW and millis() - lastPushMillis > DEBOUNCE_PUSH_MILLIS)  {
    stopped = ! stopped;
    lastPushMillis = millis();
  }
}

void updateSpeed() {
  // store speed in slot 8
  // only alter speed when going
  if (slotUpdated(8) and !stopped) {
    if (slot[8] > maxSpeedIndex) {
      slot[8] = maxSpeedIndex;
    }
    else {
      t.setPeriod(speeds[slot[8]]);
    }
  }
}

void updateDuration() {
  // store speed in slot 9
  if (slotUpdated(9) and stopped) {
    if (slot[9] < 5)
      slot[9] = 5;
  }
}

void nextSlot() {
  if (stopped) return;

  slotIndex = (slotIndex + 1) % 8;

  // short circuit sequence if found zero in slot
  if (slot[slotIndex] == 0 and slotIndex > 1) {
    slotIndex = 0;
  }
  tone(9, notes[slot[slotIndex]], slot[9]);
}

boolean slotUpdated(byte button) {
  boolean hasChanged = false;
  if (encoderPos != lastReportedPos) {
    if (encoderPos > lastReportedPos) {
      if (slot[button] < maxNotesIndex) {
        slot[button]++;
        hasChanged = true;
      }
    }
    else {
      slot[button] and slot[button]--;
      hasChanged = true;
    }
    lastReportedPos = encoderPos;
  }
  if (hasChanged and stopped) {
    tone(9, notes[slot[button]], slot[9]);
    slotIndex = button;
  }

  return hasChanged;
}


// Interrupt on A changing state
void doEncoderA() {
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change?
  if ( digitalRead(ROTATE_A) != A_set ) { // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set )
      encoderPos += 1;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if ( rotating ) delay (1);
  if ( digitalRead(ROTATE_B) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set )
      encoderPos -= 1;
    rotating = false;
  }
}

byte getButton(TM1638 * board) {
  switch (board->getButtons()) {
    case 1:   return 0;
    case 2:   return 1;
    case 4:   return 2;
    case 8:   return 3;
    case 16:  return 4;
    case 32:  return 5;
    case 64:  return 6;
    case 128: return 7;
    default:  return 8;
  }
}





