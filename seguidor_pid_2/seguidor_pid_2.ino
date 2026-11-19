#include <Servo.h>

Servo motor_esquerda;
Servo motor_direita;

// Portas dos sensores (esquerda -> direita)
const int S1 = A1;
const int S2 = A2;
const int S3 = A3; // Central
const int S4 = A4;
const int S5 = A5;

// Valores ABAIXO de 700 indicam detecção da linha
const int LIMIAR = 700;

// Ponto neutro dos servos (parado) e velocidade base de avanço
const int VEL_PARADO = 90;
const int VEL_BASE = 42; // Quanto maior, mais rápido o robô anda (ajuste com segurança)

// Ganhos do PID (comece baixo e ajuste na pista)
float Kp = 30.0;
float Ki = 0.05;
float Kd = 4.0;

// Variáveis do PID
float erro = 0;
float erro_anterior = 0;
float integral = 0;
int ultimo_lado = 0; // -1 = esquerda, +1 = direita (ajuda se perder a linha)

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

  int soma_ativos = v1 + v2 + v3 + v4 + v5;

  if (soma_ativos > 0) {
    // Calcula a posição média ponderada da linha:
    // Esquerda = valores negativos (-2, -1), Centro = 0, Direita = positivos (+1, +2)
    float soma_ponderada = (v1 * -2.0) + (v2 * -1.0) + (v3 * 0.0) + (v4 * 1.0) + (v5 * 2.0);
    erro = soma_ponderada / soma_ativos;

    if (erro < -0.2) ultimo_lado = -1;
    else if (erro > 0.2) ultimo_lado = 1;
  } else {
    // Se nenhum sensor detectou a linha, mas o robô vinha alinhado ao centro,
    // assume falha isolada do S3 e mantém erro neutro para evitar tranco brusco
    if (abs(erro_anterior) < 0.5) {
      erro = 0.0;
    } else {
      // Caso contrário, perdeu a linha de fato e puxa para o último lado visto
      erro = ultimo_lado * 2.5;
    }
  }

  // Cálculo dos termos PID
  float P = erro;

  integral += erro;
  integral = constrain(integral, -50, 50); // Evita que o integral estoure
  float I = integral;

  float derivativo = erro - erro_anterior;
  float D = derivativo;

  float correcao = (Kp * P) + (Ki * I) + (Kd * D);
  erro_anterior = erro;

  // Lembrete do seu hardware:
  // Motor esquerdo: > 90 gira para frente
  // Motor direito: < 90 gira para frente
  int cmd_esq = VEL_PARADO + VEL_BASE + correcao;
  int cmd_dir = VEL_PARADO - VEL_BASE + correcao;

  // Limita aos valores válidos do Servo.write()
  cmd_esq = constrain(cmd_esq, 0, 180);
  cmd_dir = constrain(cmd_dir, 0, 180);

  motor_esquerda.write(cmd_esq);
  motor_direita.write(cmd_dir);
}
