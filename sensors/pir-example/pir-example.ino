/*
   PIR sensor example
*/

int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
long millis_last_state_change = 0;

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(inputPin);  // read input value
  if (val != pirState) {        // has the PIR changed state?
    digitalWrite(ledPin, val);  // turn LED ON
    Serial.print(val);
    Serial.write(9);            // tab
    Serial.print(millis_last_state_change);
    Serial.write(9);            // tab
    Serial.print(millis()/1000);
    Serial.write(9);            // tab
    Serial.println(millis() - millis_last_state_change);
    pirState = val;
    millis_last_state_change = millis();
  }
}

