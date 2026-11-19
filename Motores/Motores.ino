#include <Servo.h>                           // inclui a biblioteca do Motor

Servo motor_esquerda;                       // Falo que tenho um servo com nome "motor_esquerda"
Servo motor_direita;                        // Falo que tenho um servo com nome "motor_direita"

void setup() {                              // Vamos dizer ao arduino os componentes que temos que programar
  motor_esquerda.attach(8);                 // Falamos em qual porta digital que conectamos o motor_esquerda (Em caso de porta diferente, trocar o número)
  motor_direita.attach(9);                // Falamos em qual porta digital que conectamos o motor_direita  (Em caso de porta diferente, trocar o número)
}

void loop() {                               // Vamos iniciar o ciclo de ações do motor
  motor_esquerda.write(45);                 // Colocamos a velocidade do motor_esquerda
  motor_direita.write(135);                 // Colocamos a velocidade do motor_direita
  delay(5000);                              // Utilizamos delay para dizer em quanto tempo as ações anteriores deverão ser feitas, o tempo está em milissegundos, ou seja, 1000 ms = 1 segundo
}

// Lembrete sobre a velocidade
// 90 -> o motor fica parado
// Acima de 90 até 180 -> motor roda no sentido horário - quanto mais próximo de 180 mais rápido
// Abaixo de 90 até 0 -> motor roda no sentido anti-horário - quanto mais próximo de 0 mais rápido
