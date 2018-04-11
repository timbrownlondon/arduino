const byte ledPin = 13;
const byte interruptPin = 2; // for example attach to PIR sensor or push switch
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
}

void loop() {
  digitalWrite(ledPin, state);
  delay(10);
}

void blink() {
  state = !state;
}
