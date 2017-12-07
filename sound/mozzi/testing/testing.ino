#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#include <TM1638.h>
TM1638 led_board(5, 6, 7, true, 0); // data, clock, strobe, activate, brightness 0-7

#define CONTROL_RATE 64 // originally 128
Oscil <2048, AUDIO_RATE> aSin(SIN2048_DATA);    // default is 16384
Oscil <2048, CONTROL_RATE> kVib(SIN2048_DATA);  // default is 64 (can set to 128 or 256  etc.)

#define POT_0 0
float centre_freq = 440.0;
float depth = 0.25;

void setup() {
  kVib.setFreq(6.5f);
  startMozzi(CONTROL_RATE);
}

void updateControl() {

  centre_freq = mozziAnalogRead(POT_0);
  float vibrato = depth * kVib.next();
  aSin.setFreq(centre_freq + vibrato);

  led_board.setDisplayToDecNumber(AUDIO_RATE, 0, false);

}

int updateAudio() {
  return aSin.next();
}

void loop() {
  audioHook();
}
