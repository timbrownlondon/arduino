
#include <TM1638.h>
TM1638 led_board(5, 6, 7, true, 0); // data, clock, strobe, activate, brightness 0-7
uint32_t count = 1;


#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);



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

  // led_board.setDisplayToDecNumber(fundamental, 0, false);


  wavey.set(fundamental, bandwidth, centre_freq);
}

int updateAudio() {
  return wavey.next() >> 8; // >>8 for AUDIO_MODE STANDARD

}

void loop() {
  audioHook(); // required here
}
