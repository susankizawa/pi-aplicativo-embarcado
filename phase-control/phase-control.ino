// C++ code
//

#define pot A0
#define zc 2
#define triac 9

int zeroCross = 0;
int last_state = 0;

void setup() {
  // set up pins
  pinMode(pot, INPUT);        // potentiometer input
  pinMode(triac, OUTPUT);    // triac gate control
  
  Serial.begin(9600);
  
  // set up zero crossing interrupt
  attachInterrupt(digitalPinToInterrupt(zc), zeroCrossingInterrupt, RISING);
}

void loop() {
  int potValue = analogRead(pot);
  int phaseDelay = map(potValue, 0, 1023, 16.66, 1);
  
  Serial.print("phaseDelay: ");
  Serial.println(phaseDelay);
  
  if(zeroCross){
    delay(phaseDelay); // this controls the power
    digitalWrite(triac, HIGH);
    delay(1);
    digitalWrite(triac, LOW);
    zeroCross = 0;
  }
}

// Interrupt Service Routines

void zeroCrossingInterrupt() {
  zeroCross = 1;
}