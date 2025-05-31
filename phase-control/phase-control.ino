#include <avr/io.h>
#include <avr/interrupt.h>

#define DETECT 2
#define GATE 9
#define PULSE_WIDTH_COUNTS 63
#define POT A0

volatile int phaseDelayCounts = 0;

void setup(){
  pinMode(DETECT, INPUT_PULLUP);
  pinMode(POT, INPUT);
  pinMode(GATE, OUTPUT);

  TCCR1A = 0x00;
  TCCR1B = 0x00;

  OCR1A = 1;
  OCR1B = 1 + PULSE_WIDTH_COUNTS;

  TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1);

  attachInterrupt(digitalPinToInterrupt(DETECT), zeroCrossingInterrupt, RISING);

  Serial.begin(9600);
  Serial.println("AC Control V1.2 Initialized");
}

void zeroCrossingInterrupt(){
  TCCR1B = 0x00;

  TCNT1 = 0;

  TCCR1B = (1 << CS12);

  digitalWrite(GATE, LOW);
}

ISR(TIMER1_COMPA_vect){
  digitalWrite(GATE, HIGH);

  OCR1B = TCNT1 + PULSE_WIDTH_COUNTS;
}

ISR(TIMER1_COMPB_vect){
  digitalWrite(GATE, LOW);
  TCCR1B = 0x00;
}

void loop(){
  int potValue = analogRead(POT);

  int phaseDelay = map(potValue, 0, 1023, 480, 5);

  OCR1A = phaseDelay;

  delay(15);
}