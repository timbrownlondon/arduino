#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int direction = 1;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void __loop(){
  myservo.write(0);
  delay(2000);

  myservo.write(180);
  delay(2000);
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
  myservo.write(pos);
  delay(10);       // wait between 1 and 10 seconds
}
