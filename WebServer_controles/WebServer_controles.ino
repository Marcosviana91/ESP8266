#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
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
int PORTAS[] = {16, 5, 4, 0, 2, 14, 12, 13, 15, 3, 1};// 11 portas

int porta_atual, porta_anterior;
char funcao_atual = 't';//t: testar() p: piscar() a: acender() (um por vez)
char leitura;

int paraInt(char C);//transforma um char numérico em inteiro.
void piscar();
void acender();
void testar();

int tempo = 500;

void apagarTudo() {
  for (int x = 0; x < 11; x++) {
    digitalWrite(PORTAS[x], 0);
  }
  Serial.println("Apagou tudo!");
}
//A0 porta analógica

const char* ssid = "VIVOFIBRA-5A70";
const char* password = "778D54EB67";

ESP8266WebServer server(80);

const int led = 2;
//Minha edição
void controles() {
  String textoHTML;//inserir o código HTML aqui.

  textoHTML = "<h1>Status das portas:</h1><br>";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);
  textoHTML += "<br>";
  textoHTML += "Porta digital: ";
  for (int p=0; p<=11; p++) {
    textoHTML += "Porta D";
    textoHTML += p;
    textoHTML += ": ";
    textoHTML += digitalRead(PORTAS[p]);
    textoHTML += "<br>";
  }
  
  server.send(200, "text/html", textoHTML);
}
//Fim da minha edição
void handleRoot() {
  digitalWrite(led, 1);

  String pagina_principal_HTML;
  String title_pagina_HTML = "Página Principal";
  
  //Adicione aqui o CSS
  String pagina_principal_CSS = "h1 {color: blue;}";
  
  //Adicione aqui o conteúdo do corpo da página
  String pagina_body_HTML = "<h1>Bem-Vindo!</h1><hr>";
  
  //Adiciona o cabeçalho HTML
  pagina_principal_HTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  pagina_principal_HTML  += "<title>" + title_pagina_HTML + "</title><style>" + pagina_principal_CSS + "</style></head><body>" + pagina_body_HTML + "</body></html>";
  
  server.send(200, "text/html", pagina_principal_HTML);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
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
  digitalWrite(led, 0);
  //Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/controles", controles);//direciona a pagina para uma função

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });//assim a função inteira está na chamada

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  server.handleClient();
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
