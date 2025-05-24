// C++ code
//
int turnedOn = 0;
const int pin = 3;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(pin, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  // pega tempo em q o pino estava high e low
  unsigned long highTime = pulseIn(pin, HIGH);
  unsigned long lowTime = pulseIn(pin, LOW);

  // caso high e low nn sejam zeros
  if(highTime != 0 && lowTime != 0) {
    // pega o periodo e dps a frequencia
    float period = highTime + lowTime; // in microseconds
    float frequency = 1000000.0 / period; // Hz
  
    // logsss
    Serial.print("PWM Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");
  }

  // mapeamento dos valores analogicos para digitais
  int mappedValue = map(analogRead(A0), 0, 1023, 0, 255);
  Serial.println(mappedValue);
  analogWrite(pin, mappedValue);
  delay(10);
}