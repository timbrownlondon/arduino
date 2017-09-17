#include <Servo.h>
#include <NewPing.h>

// sonar setup
#define ECHO_PIN       2
#define TRIGGER_PIN    3
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// servo setup
Servo servo;
int pos = 0;    // servo position
int direction = 1;

void setup() {
  servo.attach(9);  // servo control (orange) on pin 9
  Serial.begin(9600);

}

void loop() {
  pos +=  direction;
  if (pos > 179 ) {
    pos = 179;
    direction = -1;
  }

  if (pos < 0) {
    pos = 0;
    direction = 1;
  }
  servo.write(pos);
  delay(250);

  Serial.print(pos);
  Serial.print(" ");
  Serial.print(sonar.ping_cm());
  Serial.println(" cm");

}
