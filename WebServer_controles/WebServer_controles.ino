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
char funcao_atual = 'p';//t: testar() p: piscar() a: acender() (um por vez)
String leitura;

void lerSerial();
void piscar();
void acender();
void testar();

int tempo = 500;

void apagarTudo() {
  for (int x = 0; x < 11; x++) {
    digitalWrite(PORTAS[x], 0);
  }
}

//const char* ssid = "Marcos Viana";
//const char* password = "12345677";

const char* ssid = "VIVOFIBRA-5A70";
const char* password = "778D54EB67";

ESP8266WebServer server(80);

void handleRoot() {
  String pagina_Head = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>ESP8266 | Página Principal.</title><meta http-equiv='refresh' content='2'><style>* {margin: 0px;padding: 0px;}body {background-color: aqua;}h1,h2,h3 {text-align: center;padding: 5px;}p {padding: 3px;}hr {width: 75%;margin: 10px auto;border-color: aquamarine;}#GPIOs,#funcoes {margin: 10px;padding: 5px 10px;border: 1px solid black;border-radius: 15px;background-color: aquamarine;}.ligado>strong {color: blue;text-transform: uppercase;}.desligado>strong {color: red;text-transform: uppercase;}input {text-align: right;background-color: aqua;}footer>nav {background-color: rgb(0, 112, 112);padding: 10px;}footer>nav>a {transition: 200ms;margin-right: 10px;color: black;text-decoration: none;padding: 5px;border-radius: 5px;}footer>nav>a:hover {color: black;background-color: aquamarine;}footer>nav>a:visited {color: rgb(26, 26, 26);text-decoration: none;}</style></head>";
  String pagina_Body = "<body><header><h1>Bem-Vindo!</h1><p>Está é a página principal do ESP8266, construída pela função <strong>handleRoot()</strong>. Para modificaresta página, modifique esta função.</p><hr></header>";
  String pagina_Main = "<main><h2>Estado das GPIOs</h2><section id='GPIOs'>";
  
  pagina_Main += "<p>A0: <input value='";
  pagina_Main += analogRead(A0);
  pagina_Main += "' readonly></p>";
  for (int p = 0; p < 11; p++)
  {
    String classeName = (digitalRead(PORTAS[p])) ? "ligado" : "desligado";
    pagina_Main += "<p class='" + classeName + "'>D" +p+ ": <strong>" +classeName+ "</strong>.</p>";
  }
  pagina_Main += "</section></main>";
  String pagina_Footer = "<footer><hr><br><nav><a href='../'>Início</a><a href='./controles'>Controles</a></nav></footer></body></html>";
  
  //pagina_Body $text_GPIO_D0$ $name_GPIO_D0$
  String pagina_final = pagina_Head + pagina_Body + pagina_Main + pagina_Footer;

//  for (int portas = 0; portas <= 11; portas++) {
//    pagina.Body.
//  }
  digitalWrite(D4, !digitalRead(D4));
  server.send(200, "text/html", pagina_final);
  digitalWrite(D4, !digitalRead(D4));
}

void handleNotFound() {
  digitalWrite(D4, !digitalRead(D4));
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
  digitalWrite(D4, !digitalRead(D4));
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
