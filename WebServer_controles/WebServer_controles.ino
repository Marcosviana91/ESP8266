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

//Placa atual NodeMCU ou ESP01 (false)
String device = "ESP8266";
int num_portas = 4;
//String device = "ESP01";

int porta_atual, porta_anterior;
char funcao_atual = 'p';//t: testar() p: piscar() a: acender() (um por vez)
String leitura;

void lerSerial();
void piscar();
void acender();
void testar();

int tempo = 500;

void apagarTudo() {
  for (int x = 0; x < num_portas; x++) {
    digitalWrite(PORTAS[x], 0);
  }
}

//const char* ssid = "Marcos Viana";
//const char* password = "12345677";

const char* ssid = "VIVOFIBRA-5A70";
const char* password = "778D54EB67";

ESP8266WebServer server(80);

void handleRoot() {
  String pagina_Head = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" + device + " | Página Principal.";
  pagina_Head += "</title><meta http-equiv='refresh' content='2'><style>* {margin: 0px;padding: 0px;}body {background-color: aqua;}h1,h2,h3 {text-align: center;padding: 5px;}p {padding: 3px;}hr {width: 75%;margin: 10px auto;border-color: aquamarine;}.lista {max-width: 60%;margin: 20px auto;padding: 5px 10px;border: 1px solid black;border-radius: 15px;background-color: aquamarine;}.ligado>strong {color: blue;text-transform: uppercase;}.desligado>strong {color: red;text-transform: uppercase;}.funcoes>form {display: inline;}button {transition-duration: 100ms;border-radius: 5px;border: 1px solid black;padding: 5px;margin: 2px;box-shadow: 1px 1px black;}button:hover {background-color: rgb(0, 138, 138);color: white;box-shadow: inset 1px 1px black;}input {text-align: right;background-color: aqua;}</style></head>";
  String pagina_Body = "<body><header><h1>Bem-Vindo!</h1><p>Está é a página principal do " + device + ", construída pela função <strong>handleRoot()</strong>. Para modificaresta página, modifique esta função.</p><hr></header>";
  String pagina_Main = "<main><section class='lista'><h2>Estado das GPIOs</h2>";

  if (device == "ESP8266") {
    pagina_Main += "A0: <input value='";
    pagina_Main += analogRead(A0);
    pagina_Main += "' readonly>";
    num_portas = 11;
  }
  pagina_Main += "<div id='portas'>";
  for (int p = 0; p < num_portas; p++)
  {
    String classeName = (digitalRead(PORTAS[p])) ? "" : "des";
    pagina_Main += "<form action='' method='post'><input type='hidden' name='porta' value='" + String(p) + "'>D" +String(p)+ ":<button class='";
    classeName = (digitalRead(PORTAS[p])) ? "des" : "";
    pagina_Main += classeName+"ligado'><strong>"+classeName+"ligar</strong></button></form>";
  }
  pagina_Main += "</div></section>";
  pagina_Main += "<section class='lista'><h3>FUNÇÕES:</h3><div class='funcoes'><form action='' method='post'><input type='hidden' name='funcao' value='a'><button>Manter Aceso</button></form><form action='' method='post'><input type='hidden' name='funcao' value='p'><button>Manter piscando</button></form><form action='' method='post'><input type='hidden' name='funcao' value='t'><button>Apenas testar</button></form><form action='' method='post'><input type='hidden' name='porta' value='-1'><button>Desligar Tudo</button></form></div></section></main>";
  String pagina_Footer = "</body></html>";

  //pagina_Body $text_GPIO_D0$ $name_GPIO_D0$
  String pagina_final = pagina_Head + pagina_Body + pagina_Main + pagina_Footer;

  for (int i = 0; i < server.args(); i++) {
    if ( (server.argName(i) == "porta") )
    {
      porta_atual = server.arg(i).toInt();
      Serial.println(porta_atual);
      if (porta_atual == porta_anterior) {
        porta_anterior = -1;
      }
    }
    else if ( (server.argName(i) == "funcao") )
    {
      funcao_atual = server.arg(i)[0];
    }
  }

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
  for (int i = 0; i < server.args(); i++) {
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
      Serial.println(WiFi.localIP());
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
