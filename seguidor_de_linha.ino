#include <Servo.h>

Servo motor_esquerda;
Servo motor_direita;

// Definição das portas dos sensores (de esquerda para direita)
const int S1 = A1;
const int S2 = A2;
const int S3 = A3; // Sensor central
const int S4 = A4;
const int S5 = A5;

// Limiar para diferenciar a linha do fundo: valores ABAIXO de 700 indicam detecção
const int LIMIAR = 700; 

// Velocidades de referência
const int VEL_PARADO = 90;
const int VEL_MAX_FRENTE_ESQ = 135; 
const int VEL_MAX_FRENTE_DIR = 45;  

void setup() {
  // Configuração das portas dos motores
  motor_esquerda.attach(7);
  motor_direita.attach(10);

  // Configuração dos pinos dos sensores como entrada
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  // Parar os motores inicialmente
  motor_esquerda.write(VEL_PARADO);
  motor_direita.write(VEL_PARADO);
  delay(1000);
}

void loop() {
  // Leitura dos sensores
  int s1 = analogRead(S1);
  int s2 = analogRead(S2);
  int s3 = analogRead(S3);
  int s4 = analogRead(S4);
  int s5 = analogRead(S5);

  // Lógica de seguimento corrigida para o sentido correto das curvas:
  
  if (s3 < LIMIAR) {
    // Linha no centro: vai para frente em linha reta
    motor_esquerda.write(130);
    motor_direita.write(50);
  } 
  else if (s2 < LIMIAR || s1 < LIMIAR) {
    // Linha indo para a esquerda: corrige virando para a esquerda
    motor_esquerda.write(90);   // Esquerdo desacelera/para
    motor_direita.write(45);    // Direito empurra para a frente
  } 
  else if (s4 < LIMIAR || s5 < LIMIAR) {
    // Linha indo para a direita: corrige virando para a direita
    motor_esquerda.write(135);  // Esquerdo empurra para a frente
    motor_direita.write(90);    // Direito desacelera/para
  } 
  else {
    // Se perder a linha momentaneamente, avança devagar procurando
    motor_esquerda.write(110);
    motor_direita.write(70);
  }
}
