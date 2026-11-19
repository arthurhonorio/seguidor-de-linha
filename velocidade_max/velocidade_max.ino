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

// Velocidades de referência (ajuste conforme a direção real das rodas no seu chassi)
const int VEL_PARADO = 90;
const int VEL_MAX_FRENTE_ESQ = 180; // Velocidade máxima para frente (motor esquerdo > 90)
const int VEL_MAX_FRENTE_DIR = 0;   // Velocidade máxima para frente (motor direito < 90)
const int VEL_RE_ESQ = 0;           // Sentido oposto para giro rápido / curva fechada
const int VEL_RE_DIR = 180;         // Sentido oposto para giro rápido / curva fechada

// Guarda o último lado em que a linha foi vista (-1 = esquerda, +1 = direita, 0 = centro)
int ultimo_lado = 0;

// Retorna a mediana de 3 leituras analógicas para evitar falhas pontuais e ruídos
int leituraMediana3(int pino) {
  int a = analogRead(pino);
  int b = analogRead(pino);
  int c = analogRead(pino);

  if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
  if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
  return c;
}

void setup() {
  motor_esquerda.attach(8);
  motor_direita.attach(9);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  motor_esquerda.write(VEL_PARADO);
  motor_direita.write(VEL_PARADO);
  delay(1000);
}

void loop() {
  // Lê os sensores filtrados e converte para 1 (viu a linha) ou 0 (não viu)
  int v1 = (leituraMediana3(S1) < LIMIAR) ? 1 : 0;
  int v2 = (leituraMediana3(S2) < LIMIAR) ? 1 : 0;
  int v3 = (leituraMediana3(S3) < LIMIAR) ? 1 : 0;
  int v4 = (leituraMediana3(S4) < LIMIAR) ? 1 : 0;
  int v5 = (leituraMediana3(S5) < LIMIAR) ? 1 : 0;

  // Caso 1: Linha no centro (S3 detectou)
  if (v3 == 1) {
    ultimo_lado = 0;
    motor_esquerda.write(VEL_MAX_FRENTE_ESQ);
    motor_direita.write(VEL_MAX_FRENTE_DIR);
  }
  // Caso 2: Linha à esquerda (S1 ou S2 detectou) -> esterça para a esquerda
  else if (v2 == 1 || v1 == 1) {
    ultimo_lado = -1;
    // Se estiver bem na ponta (S1), gira mais forte para não escapar da curva
    if (v1 == 1) {
      motor_esquerda.write(VEL_RE_ESQ);
      motor_direita.write(VEL_MAX_FRENTE_DIR);
    } else {
      motor_esquerda.write(VEL_PARADO);
      motor_direita.write(VEL_MAX_FRENTE_DIR);
    }
  }
  // Caso 3: Linha à direita (S4 ou S5 detectou) -> esterça para a direita
  else if (v4 == 1 || v5 == 1) {
    ultimo_lado = 1;
    // Se estiver bem na ponta (S5), gira mais forte para não escapar da curva
    if (v5 == 1) {
      motor_esquerda.write(VEL_MAX_FRENTE_ESQ);
      motor_direita.write(VEL_RE_DIR);
    } else {
      motor_esquerda.write(VEL_MAX_FRENTE_ESQ);
      motor_direita.write(VEL_PARADO);
    }
  }
  // Caso 4: Nenhum sensor detectou a linha no momento
  else {
    // Se o último estado conhecido era centralizado, assume possível falha isolada do S3
    // e mantém velocidade máxima em linha reta para evitar tranco brusco
    if (ultimo_lado == 0) {
      motor_esquerda.write(VEL_MAX_FRENTE_ESQ);
      motor_direita.write(VEL_MAX_FRENTE_DIR);
    }
    // Se vinha de curva à esquerda, busca a linha virando para a esquerda
    else if (ultimo_lado < 0) {
      motor_esquerda.write(VEL_RE_ESQ);
      motor_direita.write(VEL_MAX_FRENTE_DIR);
    }
    // Se vinha de curva à direita, busca a linha virando para a direita
    else {
      motor_esquerda.write(VEL_MAX_FRENTE_ESQ);
      motor_direita.write(VEL_RE_DIR);
    }
  }
}
