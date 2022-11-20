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
char funcao_atual = 'a';//t: testar() p: piscar() a: acender() (um por vez)
String leitura;

void lerSerial();
void piscar();
void acender();
void testar();
String montarPagina(String titulo, String contuedo);

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
  String textoHTML = "<h2>Controle das GPIOs</h2><div>";//inserir o código HTML aqui.
  if (server.args() > 0) {
    String message = "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (short int i = 0; i < server.args(); i++) {
      message += String(server.argName(i));
      message += ": " + String(server.arg(i));
      message += "\n";
      Serial.println(message);
      if (server.argName(i) == "porta") {
        if (isDigit(server.arg(i)[0])) {
          porta_atual = String(server.arg(i)[0]).toInt();
          if (porta_atual == porta_anterior) {
            porta_anterior = -1;
          }
        }
      } else if (server.argName(i) == "funcao") {
        if (server.arg(i) == "testar") {
          funcao_atual = 't';
        } else if (server.arg(i) == "piscar") {
          funcao_atual = 'p';
        } else if (server.arg(i) == "acender") {
          funcao_atual = 'a';
        } else if (server.arg(i) == "-") {
          porta_atual = -1;
          apagarTudo();
        }
      }
    }
  }
  for (int p = 0; p <= 11; p++) {
    textoHTML += "<p>Porta D";
    textoHTML += p;
    textoHTML += ": ";
    textoHTML += "<form action='' method='post'><input type='hidden' name='porta' value='" + String(p) + "'><button>";
    textoHTML += (digitalRead(PORTAS[p]) == 1) ? "Desligar" : "Ligar";
    textoHTML += "</button></form></p>";
  }
  textoHTML += "<p>FUNÇÕES:</p><form action='' method='post'><input type='hidden' name='funcao' value='acender'><button>Manter Aceso</button></form><br><form action='' method='post'><input type='hidden' name='funcao' value='piscar'><button>Manter piscando</button></form><br><form action='' method='post'><input type='hidden' name='funcao' value='testar'><button>Apenas testar</button></form><br><form action='' method='post'><input type='hidden' name='funcao' value='-'><button>Desligar Tudo</button></form>";
  server.send(200, "text/html", montarPagina("ESP8266 | Controles", textoHTML));
}

void handleRoot() {

  String estado_GPIOs = "<h2>Estado das GPIOs</h2><div>";
  estado_GPIOs += "<p>Porta A0: " + String(analogRead(A0)) + "</p>";
  //<p id='Ligado'>D0: <strong>Ligado</strong>. </p>
  //<p id='Desligado'>D1: <strong>Desligado</strong>. </p>
  for (int p = 0; p <= 11; p++) {
    if (digitalRead(PORTAS[p])) {
      estado_GPIOs += "<p id='Ligado'>D";
      estado_GPIOs += String(p);
      estado_GPIOs += ": <strong>Ligado</strong></p>";
    }
    else {
      estado_GPIOs += "<p id='Desligado'>D";
      estado_GPIOs += String(p);
      estado_GPIOs += ": <strong>Desligado</strong>. </p>";
    }
  }
  estado_GPIOs += "</div>";
  digitalWrite(D4, !digitalRead(D4));
  server.send(200, "text/html", montarPagina("ESP8266 | Página Principal", estado_GPIOs));
  digitalWrite(D4, !digitalRead(D4));
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

String montarPagina(String titulo, String contuedo) {
  String estilo_CSS = "body>p {padding: 0px 10px;text-indent: 25px;text-align: justify;}header > hr {padding: 0px;margin-top: -0.8em;margin-bottom: 2em;}h1 {color: blue;text-align: center;}main#estados {max-width: 80%;margin: auto;}main>div {border: 2px solid black;border-radius: 15px;padding: 10px;background-color: aquamarine;}main > div > p { !importantpadding: 0px;margin: 0px;}form {display: inline;}p#Ligado strong {color: blue;}p#Ligado strong::after {content: '✔️';}p#Desligado strong {color: red;}p#Desligado strong::after {content: '✖️';}footer > hr {padding: 0px;margin-top: 2em;margin-bottom: -0.8em;}footer a {border: 1px solid black;border-radius: 10%;background-color: aquamarine;padding: 2px 5px;margin: 0px 5px;}";
  String HTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" + titulo + "</title><style>" + estilo_CSS + "</style></head>";
  HTML += "<body><header><h1>Bem-Vindo!</h1><p>Está é a página <strong>" + titulo + "</strong> do ESP8266.</p><hr></header><main>" + contuedo + "</main><footer><hr><br><nav><a href='../'>Início</a><a href='./controles'>Controles</a></nav></footer></body></html>";

  return HTML;
}
