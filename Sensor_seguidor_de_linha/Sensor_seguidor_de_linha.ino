// Sensores no sentido esquerda -> direita

const int S1 = A1;         // Indicamos que o sensor 1 está conectado na porta analógica A1
const int S2 = A2;         // Indicamos que o sensor 2 está conectado na porta analógica A2
const int S3 = A3;         // Indicamos que o sensor 3 está conectado na porta analógica A3
const int S4 = A4;         // Indicamos que o sensor 4 está conectado na porta analógica A4
const int S5 = A5;         // Indicamos que o sensor 5 está conectado na porta analógica A5

void setup() {
  Serial.begin(9600);      // Inicia a comunicação com o computador

  pinMode(S1, INPUT);      // Diz ao Arduino que o S1 vai receber informações
  pinMode(S2, INPUT);      // Diz ao Arduino que o S2 vai receber informações
  pinMode(S3, INPUT);      // Diz ao Arduino que o S3 vai receber informações
  pinMode(S4, INPUT);      // Diz ao Arduino que o S4 vai receber informações
  pinMode(S5, INPUT);      // Diz ao Arduino que o S5 vai receber informações
}

void loop() {

  int s1 = analogRead(S1); // Lê o valor do sensor S1
  int s2 = analogRead(S2); // Lê o valor do sensor S2
  int s3 = analogRead(S3); // Lê o valor do sensor S3
  int s4 = analogRead(S4); // Lê o valor do sensor S4
  int s5 = analogRead(S5); // Lê o valor do sensor S5

  Serial.print(s1);        // Escreve na tela o valor que s1 está lendo
  Serial.print(" ");       // Escreve na tela o valor um espaço
  Serial.print(s2);        // Escreve na tela o valor que s2 está lendo
  Serial.print(" ");       // Escreve na tela o valor um espaço
  Serial.print(s3);        // Escreve na tela o valor que s3 está lendo
  Serial.print(" ");       // Escreve na tela o valor um espaço
  Serial.print(s4);        // Escreve na tela o valor que s4 está lendo
  Serial.print(" ");       // Escreve na tela o valor um espaço
  Serial.println(s5);      // Escreve na tela o valor que s5 está lendo e pula linha

}

