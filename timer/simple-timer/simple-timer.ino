/*
  Example Timer1 Interrupt
  toggle LED every second
*/

#define ledPin 13

unsigned long count = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  noInterrupts();           // disable interrupts

  //http://www.instructables.com/id/Arduino-Timer-Interrupts/
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts();             // enable  interrupts
}

// interrupt service routine
ISR(TIMER1_COMPA_vect) {
  //timer1 interrupt 1Hz toggles pin 13 (LED)
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  count++;
}

void loop() {
  delay(3000);
  Serial.print(count);
  Serial.print("\t");
  Serial.println(millis());
}
