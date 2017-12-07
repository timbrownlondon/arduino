/*
  Example of Wavepacket synthesis, using analog
  inputs to change the fundamental frequency,
  bandwidth and centre frequency,
  using Mozzi sonification library.

  Demonstrates WavePacket, mozziAnalogRead(), and smoothing
  control signals with RollingAverage.
  Also demonstrates AutoMap, which maps unpredictable inputs to a set range.

  This example goes with a tutorial on the Mozzi site:
  http://sensorium.github.io/Mozzi/learn/introductory-tutorial/

  The circuit:
    Audio output on digital pin 9 on a Uno or similar.

    Potentiometers connected to analog pins 0, 1 ,2
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground

  Mozzi help/discussion/announcements:
  https://groups.google.com/forum/#!forum/mozzi-users

*/

#include <WavePacket.h>
#include <AutoMap.h>

#include <TM1638.h>
TM1638 led_board(5, 6, 7, true, 0); // data, clock, strobe, activate, brightness 0-7
uint32_t count = 1;

// define min/max fundemantal frequency for potentiometer attached to A0
#define POT_FD 0
#define MIN_FD 1
#define MAX_FD 100

// define min/max bandwidth for potentiometer attached to A1
#define POT_BW 1
#define MIN_BW 1
#define MAX_BW 1000

// define min/max centre frequency for potentiometer attached to A2
#define POT_CF 2
#define MIN_CF 60   // original 60
#define MAX_CF 2000 // original 2000

AutoMap kMapFd(0, 1023, MIN_FD, MAX_FD);
AutoMap kMapBw(0, 1023, MIN_BW, MAX_BW);
AutoMap kMapCf(0, 1023, MIN_CF, MAX_CF);

WavePacket <DOUBLE> wavey; // DOUBLE selects 2 overlapping streams
//WavePacket <SINGLE> wavey;

void setup() {
  // wait before starting Mozzi to receive analog reads, so AutoRange will not get 0
  led_board.setDisplayToDecNumber(count, 0, false);
  delay(200);
  startMozzi();
}

void updateControl() {
  int fundamental = kMapFd(mozziAnalogRead(POT_FD));
  int bandwidth   = kMapBw(mozziAnalogRead(POT_BW));
  int centre_freq = kMapCf(mozziAnalogRead(POT_CF));

  led_board.setDisplayToDecNumber(fundamental, 0, false);


  wavey.set(fundamental, bandwidth, centre_freq);
}

int updateAudio() {
  return wavey.next() >> 8; // >>8 for AUDIO_MODE STANDARD

}

void loop() {
  audioHook(); // required here

}
