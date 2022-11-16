#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//Portas internas
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2//LED_BUILTIN
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1
int PORTAS[] = {16, 5, 4, 0, 2, 14, 12, 13, 15, 3, 1};// 11 portas
//A0 porta analógica

const char* ssid = "VIVOFIBRA-5A70";
const char* password = "778D54EB67";

ESP8266WebServer server(80);

const int led = 2;
//Minha edição
void controles() {
  digitalWrite(led, 1);

  String textoHTML;//inserir o código HTML aqui.

  textoHTML = "Status das portas:<br>";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);

  server.send(200, "text/html", textoHTML);
  digitalWrite(led, 0);
}
//Fim da minha edição
void handleRoot() {
  digitalWrite(led, 1);

  String pagina_principal_HTML;
  String title_pagina_HTML = "Página Principal";
  
  //Adicione aqui o CSS
  String pagina_principal_CSS = "h1 {color: blue;}";
  
  //Adicione aqui o conteúdo do corpo da página
  String pagina_body_HTML = "<h1>Título h1</h1><hr>";
  
  //Adiciona o cabeçalho HTML
  pagina_principal_HTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  pagina_principal_HTML  += "<title>" + title_pagina_HTML + "</title><style>" + pagina_principal_CSS + "</style></head><body>" + pagina_body_HTML + "</body></html>";
  
  pagina_body_HTML += "Porta D0: " + String(digitalRead(D0)) + " Outra coisa";
  
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
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
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
}
