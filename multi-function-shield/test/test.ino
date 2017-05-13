#include <MultiFuncShield.h>




int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;

void setup() {
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  Timer1.initialize();
  MFS.initialize(&Timer1); // initialize multi-function shield library

  // NOTE beep control is performed in the background, i.e. beep() is non blocking.

  // short beep for 200 milliseconds
  // MFS.beep();

  delay(1000);

  MFS.write("Hi");
  delay(2000);
  MFS.write(-273);
  delay(2000);
  MFS.write(3.141, 3); // display to 2 decimal places.
  delay(2000);
}

void alert() {
  // 4 short beeps, repeated 3 times.
  MFS.beep(5, // beep for 50 milliseconds
           5, // silent for 50 milliseconds
           4, // repeat above cycle 4 times
           3, // loop 3 times
           50 // wait 500 milliseconds between loop
          );
}

void loop() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  delay(1000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(1000);
}
