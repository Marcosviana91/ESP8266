#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
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
//A0 porta analógica
int PORTAS[] = {16, 5, 4, 0, 2, 14, 12, 13, 15, 3, 1};// 11 portas

int porta_atual, porta_anterior;
char funcao_atual = 't';//t: testar() p: piscar() a: acender() (um por vez)
String leitura;

void piscar();
void acender();
void testar();
void lerSerial();

int tempo = 500;

void apagarTudo() {
  for (int x = 0; x < 11; x++) {
    digitalWrite(PORTAS[x], 0);
  }
}

const char* ssid = "Marcos Viana";
const char* password = "12345677";

ESP8266WebServer server(80);

void controles() {
  String textoHTML;//inserir o código HTML aqui.

  textoHTML = "<h1>Status das portas:</h1><br>";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);
  textoHTML += "<br>";
  textoHTML += "Porta digital: <br>";
  for (int p = 0; p <= 11; p++) {
    textoHTML += "Porta D";
    textoHTML += p;
    textoHTML += ": ";
    textoHTML += digitalRead(PORTAS[p]);
    textoHTML += "<br>";
  }

  if (server.args() > 0) {
    for (short int i = 0; i < server.args(); i++) {
      if (server.argName(i) == "porta") {
        if (isDigit(server.arg(i)[0])) {
          porta_atual = String(server.arg(i)[0]).toInt();
          if (porta_atual == porta_anterior) {
            porta_anterior = -1;
          }
        }
      }
    }
  }

  server.send(200, "text/html", textoHTML);
}

void handleRoot() {

  String pagina_principal_HTML;
  String title_pagina_HTML = "ESP8266 | Página Principal";
  String estado_GPOIs = "";

  //Adicione aqui o CSS
  String pagina_principal_CSS = "h1 {color: blue;text-align: center;}body>p {padding: 0px 10px;text-indent: 25px;text-align: justify;}div#estados {max-width: 80%;margin: auto;}div#GPIOs>p{ !important padding: 0px;margin: 0px;}div#GPIOs {border: 2px solid black;border-radius: 15px;padding: 10px;background-color: aquamarine;}p#Ligado strong {color: blue;}p#Ligado strong::after {content: '✔️';}p#Desligado strong {color: red;}p#Desligado strong::after {content: '✖️';}";

  //<p id='Ligado'>D0: <strong>Ligado</strong>. </p>
  //<p id='Desligado'>D1: <strong>Desligado</strong>. </p>
  for (int p = 0; p <= 11; p++) {
    if (digitalRead(PORTAS[p])) {
      estado_GPOIs += "<p id='Ligado'>D";
      estado_GPOIs += String(p);
      estado_GPOIs += ": <strong>Ligado</strong>. </p>";
    }
    else {
      estado_GPOIs += "<p id='Desligado'>D";
      estado_GPOIs += String(p);
      estado_GPOIs += ": <strong>Desligado</strong>. </p>";
    }
  }
  //Adicione aqui o conteúdo do corpo da página
  String pagina_body_HTML = "<h1>Bem-Vindo!</h1><p>Está é a página principal do ESP8266, construída pela função <strong>handleRoot()</strong>. Para modificar esta página, modifique esta função.</p><hr><div id='estados'><h2>Estado das GPIOs</h2><p>Estado atual das GPIOs:</p><div id='GPIOs'>" + estado_GPOIs + "</div></div>";
  pagina_body_HTML += "<footer><p>Visualizar página de <a href='/controles'>Controles</a>.</p></footer>";

  //Adiciona o cabeçalho HTML
  pagina_principal_HTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  pagina_principal_HTML  += "<title>" + title_pagina_HTML + "</title><style>" + pagina_principal_CSS + "</style></head><body>" + pagina_body_HTML + "</body></html>";

  digitalWrite(D4, 1);
  server.send(200, "text/html", pagina_principal_HTML);
  digitalWrite(D4, 0);
}

void handleNotFound() {
  digitalWrite(D4, 1);
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
  digitalWrite(D4, 0);
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
  pinMode(A0, INPUT);
  Serial.begin(19200);
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

  server.on("/", handleRoot);
  server.on("/controles", controles);//direciona a pagina para uma função
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  server.handleClient();
  lerSerial();

  switch (funcao_atual) {
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

void lerSerial() {
  if (Serial.available() > 0)
  {
    leitura = Serial.readStringUntil(' ');
    //Verifica se é número
    if (leitura == "testar") {
      funcao_atual = 't';
    } else if (leitura == "piscar") {
      funcao_atual = 'p';
    } else if (leitura == "acender") {
      funcao_atual = 'a';
    } else if (leitura == "-") {
      porta_atual = -1;
      apagarTudo();
    } else if (isDigit(leitura[0])) {
      porta_atual = leitura.toInt();
      Serial.println("Porta: " + porta_atual);
      if (porta_atual == porta_anterior) {
        porta_anterior = -1;
      }
    } else {
      Serial.println("Comando não reconhecido.");
    }
  }
}

void testar() {//Liga e desliga a porta por 500 milisegundos
  apagarTudo();
  if (porta_atual != porta_anterior && porta_atual >= 0)
  {
    digitalWrite(PORTAS[porta_atual], 1);
    delay(500);
    porta_anterior = porta_atual;
    apagarTudo();
  }
}
void acender() {//Liga porta
  if (porta_atual != porta_anterior && porta_atual >= 0)
  {
    apagarTudo();
    porta_anterior = porta_atual;
  }
  digitalWrite(PORTAS[porta_atual], 1);
}
void piscar() {//Liga e desliga a porta por 500 milisegundos repetidamente
  apagarTudo();
  if (porta_atual >= 0)
  {
    digitalWrite(PORTAS[porta_atual], 1);
    delay(500);
    apagarTudo();
    delay(500);
  }
}
