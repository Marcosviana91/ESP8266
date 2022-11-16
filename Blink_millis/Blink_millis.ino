/*Este Sketch tem por objetivo demostrar o uso da função millis() para fazer o led piscar. Também foi usada a função paraInt() para transformar char entre 0 e 9 em int entre 0 e 9.
pinMode(pino,modo) pino, INPUT, INPUT_PULLUP ou OUTPUT
digitalWrite(pino, valor) pino, HIGH ou LOW
digitalRead(pino) retorna HIGH ou LOW
analogWrite(pino, valor)
pino 3,5,6,9,10,11.
valor entre 0 e 255.
millis() retorna unsigned long segundo*1000 (mil)
micros() retorna unsigned long segundo*1000000 (um milhão)
Serial.begin(9600); //No connection established

Compilation error: No connection establishedInicia a comunicação serial com a taxa de 9600 bits por segundo.
pulseIn(pino, valor, tempo_de_espera) valor: HIGH ou LOW tempo(opcional) microsegundos (unsigned long)
*/

#define LED 2
char s;

int tempo = 1000;
unsigned long int tempoAnterior;
int paraInt(char C);

void setup()  //configurar entradas e saídas
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop()  //executas as intruções repetidas vezes
{
  if (Serial.available() > 0)
{
    s = Serial.read();
    Serial.println(s);
    tempo = paraInt(s) * 100;
    Serial.println(tempo);
    Serial.println();
    if (tempo == 0)
{
      tempo = 1000;
      Serial.println("Digite um valor entre 1 e 9!");
      
    }
    
  }
  if (millis() - tempoAnterior >= tempo)  //verifica se o tempo de execução do Arduíno menos o tempo anterior é maior ou igual ao tempo pré-definido.
{
    tempoAnterior = millis();              //redefine o tempo de começar a piscar
    digitalWrite(LED, !digitalRead(LED));  //alterna o estado do LED
    
  }
}

int paraInt(char C) {
  Serial.println("entrou na função paraInt()");
  int i = 0;
  if (C == '0') {
    i = 0;
  }
  if (C == '1') {
    i = 1;
  }
  if (C == '2') {
    i = 2;
  }
  if (C == '3') {
   i = 3;
  }
  if (C == '4') {
    i = 4;
    
  }
  if (C == '5') {
    i = 5;
    
  }
  if (C == '6') {
    i = 6;
    
  }
  if (C == '7') {
    i = 7;
    
  }
  if (C == '8') {
    i = 8;
    
  }
  if (C == '9') {
    i = 9;
    
  }
  return i;
}