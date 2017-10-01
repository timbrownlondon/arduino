#include <Servo.h>
#include <NewPing.h>

// sonar setup
#define ECHO_PIN    2
#define TRIGGER_PIN 3
#define MAX_DIST  200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DIST);

// servo setup
Servo servo;
int readings[180];
int delta = 1;
int i = 90;

void setup() {
  servo.attach(9);  // servo control (orange) on pin 9
  Serial.begin(9600);

  for (int i = 0; i < 180; i++) {
    readings[i] = MAX_DIST;
  }
}



void loop() {
  i += delta;
  if ( i < 1 or i > 179 ) {
    delta *= -1;
  }
  servo.write(i);
  delay(20);
  delta = random(2) == 1 ? -1 : 1;
}

int min_near(int pos) {
  int a = pos - 30;
  int b = pos + 30;

  int angle = pos;
  for (int i = max(pos - 10, 0); i < min(pos + 10, 180); i++) {
    if (readings[i] < readings[angle]) {
      angle = i;
    }
  }
  return angle;
}
