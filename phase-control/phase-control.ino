#include <avr/io.h>
#include <avr/interrupt.h>

#define DETECT 2
#define GATE 9
#define PULSE_WIDTH_COUNTS 63	// Duração do pulso de disparo (1ms)
#define POT A0

void setup(){
  // Configura os pinos
  pinMode(DETECT, INPUT_PULLUP);
  pinMode(POT, INPUT);
  pinMode(GATE, OUTPUT);

  // Inicializa o Timer1 no modo de operação normal e desabilita por enquanto
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  // Inicializa os valores de comparação A e B (serão atualizados no loop)
  OCR1A = 1;
  OCR1B = 1 + PULSE_WIDTH_COUNTS;

  // Habilita interrupções do Timer1 para os comparadores A e B
  TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B);

  // Habilita a interrupção por borda de subida no pino DETECT (cruzamento por zero)
  attachInterrupt(digitalPinToInterrupt(DETECT), zeroCrossingInterrupt, RISING);

  Serial.begin(9600);
  Serial.println("AC Control V1.2 Initialized");
}

void zeroCrossingInterrupt(){
  // Reinicia o contador do Timer1
  TCNT1 = 0;

  // Inicia o Timer1 com prescaler de 256 (clock dividido por 256)
  TCCR1B = (1 << CS12);

  digitalWrite(GATE, LOW);
}

ISR(TIMER1_COMPA_vect){
  // Dispara o TRIAC (ativa o gate)
  digitalWrite(GATE, HIGH);

  // Agenda o desligamento do gate após o tempo definido por PULSE_WIDTH_COUNTS
  OCR1B = TCNT1 + PULSE_WIDTH_COUNTS;
}

ISR(TIMER1_COMPB_vect){
  // Desliga o gate do TRIAC
  digitalWrite(GATE, LOW);

  // Desabilita o Timer1 até o próximo cruzamento por zero
  TCCR1B = 0x00;
}

void loop(){
  int potValue = analogRead(POT);

  // Converte o valor do potenciômetro em tempo de atraso de fase (em contagens de timer)
  // 0 = 0ms
  // 518 - PULSE_WIDTH_COUNTS = 8.288ms - a duração do pulso de disparo
  int phaseDelay = map(potValue, 0, 1023, 0, 518 - PULSE_WIDTH_COUNTS);

  // Atualiza o valor de comparação A com o novo atraso de fase
  OCR1A = phaseDelay;

  delay(15);
}