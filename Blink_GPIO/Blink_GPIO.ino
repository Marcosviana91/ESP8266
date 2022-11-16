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

//Portas internas
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2 //LED_BUILTIN
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1
//#define LED_BUILTIN 2
//A0 porta analógica
int PORTAS[] = {16, 5, 4, 0, 2, 14, 12, 13, 15, 3, 1};// 11 portas
int porta_atual, porta_anterior;
char funcao_atual = 't';//t: testar() p: piscar() a: acender() (um por vez)
char leitura;

int paraInt(char C);//transforma um char numérico em inteiro.
void piscar();
void acender();
void testar();

int tempo = 500;
unsigned long int tempoAnterior;

void apagarTudo() {
  for (int x = 0; x < 11; x++) {
    digitalWrite(PORTAS[x], 0);
  }
  Serial.println("Apagou tudo!");
}

void setup()  //configurar entradas e saídas
{
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(19200);
  Serial.println("Fim do SETUP");
}

void loop()  //executas as intruções repetidas vezes
{
  if (Serial.available() > 0)
  {
    leitura = Serial.read();
    Serial.print("Digitou: ");
    Serial.print(leitura);
//Verifica se é número    
    if (isDigit(leitura)) {
      Serial.println(", é um número.");
      porta_atual = paraInt(leitura);
      porta_anterior = -1;
    }
//Verifica se é letra
    else if (isAlpha(leitura)) {
      Serial.println(", é uma letra.");
      if (leitura == 't') {
        Serial.println("Função testar().");
        funcao_atual = 't';
      } else if (leitura == 'p') {
        Serial.println("Função piscar().");
        funcao_atual = 'p';
      } else if (leitura == 'a') {
        Serial.println("Função acender().");
        funcao_atual = 'a';
      } else {
        Serial.println("Fazendo nada.");
        funcao_atual = 'n';
      }
    }
    else if (leitura == '-') {
      porta_atual = -1;
      apagarTudo();
    }
    else {
      Serial.println("Caracter não reconhecido.");
    }
  }
  else {
    switch(funcao_atual) {
      case 't':
        testar();
        break;
      case 'p':
        piscar();
        break;
      case 'a':
        acender();
        break;
      default:
        break;
    }
  }
}
void testar() {//Liga e desliga a porta por 500 milisegundos
  if (porta_atual != porta_anterior && porta_atual >= 0)
  {
    digitalWrite(PORTAS[porta_atual], 1);
    delay(500);
    apagarTudo();
    porta_anterior = porta_atual;
  }
}
void acender() {//Liga e desliga a porta por 500 milisegundos
  if (porta_atual != porta_anterior && porta_atual >= 0)
  {
    apagarTudo();
    porta_anterior = porta_atual;
  }
  digitalWrite(PORTAS[porta_atual], 1);
}
void piscar() {
  if (porta_atual >= 0)
  {
    digitalWrite(PORTAS[porta_atual], 1);
    delay(500);
    apagarTudo();
    delay(500);
  }
}
int paraInt(char C) { //O monitor deve estar configurado para não enviar fim de linha nem retorno de carro.
  //Serial.println("entrou na função paraInt()");
  int i = 0;
  switch(C)
  {
    case '0':
      i=0;
      break;
    case '1':
      i=1;
      break;
    case '2':
      i=2;
      break;
    case '3':
      i=3;
      break;
    case '4':
      i=4;
      break;
    case '5':
      i=5;
      break;
    case '6':
      i=6;
      break;
    case '7':
      i=7;
      break;
    case '8':
      i=8;
      break;
    case '9':
      i=9;
      break;
  }
  return i;
}
