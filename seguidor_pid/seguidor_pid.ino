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
float Kp = 38.0;
float Ki = 0.1;
float Kd = 1.0;

// Variáveis do PID
float erro = 0;
float erro_anterior = 0;
float integral = 0;
int ultimo_lado = 0; // -1 = esquerda, +1 = direita (ajuda se perder a linha)

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
  // Lê os sensores e converte para 1 (viu a linha) ou 0 (não viu)
  int v1 = (analogRead(S1) < LIMIAR) ? 1 : 0;
  int v2 = (analogRead(S2) < LIMIAR) ? 1 : 0;
  int v3 = (analogRead(S3) < LIMIAR) ? 1 : 0;
  int v4 = (analogRead(S4) < LIMIAR) ? 1 : 0;
  int v5 = (analogRead(S5) < LIMIAR) ? 1 : 0;

  int soma_ativos = v1 + v2 + v3 + v4 + v5;

  if (soma_ativos > 0) {
    // Calcula a posição média ponderada da linha:
    // Esquerda = valores negativos (-2, -1), Centro = 0, Direita = positivos (+1, +2)
    float soma_ponderada = (v1 * -2.0) + (v2 * -1.0) + (v3 * 0.0) + (v4 * 1.0) + (v5 * 2.0);
    erro = soma_ponderada / soma_ativos;

    if (erro < -0.2) ultimo_lado = -1;
    else if (erro > 0.2) ultimo_lado = 1;
  } else {
    // Se nenhum sensor vê a linha, mantém um erro forte para o último lado visto
    erro = ultimo_lado * 2.5;
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
  // Se erro > 0 (linha à direita), correcao > 0 -> robô deve virar para a direita:
  // aumenta velocidade para frente do esquerdo e reduz/inverte do direito.
  int cmd_esq = VEL_PARADO + VEL_BASE + correcao;
  int cmd_dir = VEL_PARADO - VEL_BASE + correcao;

  // Limita aos valores válidos do Servo.write()
  cmd_esq = constrain(cmd_esq, 0, 180);
  cmd_dir = constrain(cmd_dir, 0, 180);

  motor_esquerda.write(cmd_esq);
  motor_direita.write(cmd_dir);
}
