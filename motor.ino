/*
 */
 
#include<WiFi.h>
#include <WebServer.h>
 
const char* AC_SSID = "Tow Mater";
const char* AC_PWD = "12345678";
 
//portas para controlar o motor A
const int in1 = 26;
const int in2 = 27;
const int in3 = 32;
const int in4 = 33;
 
WebServer server(80);
 
const char* PAGINA = R"HTML(
<h1>Server Rodando!</h1>
<button
onclick="location.href='/frente'">Frente</button>
<button
onclick="location.href='/re'">Re</button>
<button
onclick="location.href='/parar'">Parar</button>
)HTML";
 
void handleRoot(){
  server.send(200, "text/html", PAGINA);
}
void motorFrente(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
  server.send(200, "text/html", PAGINA);
}
 
void motorRe(){
  digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  server.send(200, "text/html", PAGINA);
}
 
void motorParar(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  server.send(200, "text/html", PAGINA);
}
 
void setup() {
  //configurando portas do motor A
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
 
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AC_SSID, AC_PWD);
 
  Serial.begin(115200);
  Serial.println("Criando rede WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", handleRoot);
  server.on("/frente", motorFrente);
  server.on("/re", motorRe);
  server.on("/parar", motorParar);
 
  server.begin();
   
}
 
void loop() {
    server.handleClient();
}
 
 