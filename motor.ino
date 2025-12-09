#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "TowMater_Control";
const char* password = "12345678";

WebServer server(80);


const int motorIN1 = 26;
const int motorIN2 = 27;
const int motorIN3 = 32;
const int motorIN4 = 33;

#include <ESP32Servo.h>
Servo servoFrontal;
Servo servoTraseiro;
Servo servoLateral;
Servo servoDefesa;

const int pinFrontal = 12;
const int pinTraseiro = 13;
const int pinLateral = 14;
const int pinDefesa  = 15;


void frente() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void tras() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
}

void direita() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
}

void esquerda() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void parar() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
}


void ataqueFrontal() {
  servoFrontal.write(150);
  delay(400);
  servoFrontal.write(90);
}

void ataqueTraseiro() {
  servoTraseiro.write(30);
  delay(400);
  servoTraseiro.write(90);
}

void ataqueLateral() {
  servoLateral.write(150);
  delay(400);
  servoLateral.write(90);
}

void defesa() {
  servoDefesa.write(150);
  delay(500);
  servoDefesa.write(90);
}


const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
<meta charset="UTF-8">
<title>Tow Mater</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" href="style.css">
</head>
<body>

<h1>Tow Mater</h1>

<script>
function send(cmd){
  fetch(cmd);
}
</script>

</body>
</html>
)=====";


void setupServer() {
  
  server.on("/", []() {
    server.send(200, "text/html", MAIN_page);
  });


  server.on("/frente", []() { frente(); server.send(200, "text/plain", "ok"); });
  server.on("/tras", []() { tras(); server.send(200, "text/plain", "ok"); });
  server.on("/esquerda", []() { esquerda(); server.send(200, "text/plain", "ok"); });
  server.on("/direita", []() { direita(); server.send(200, "text/plain", "ok"); });
  server.on("/parar", []() { parar(); server.send(200, "text/plain", "ok"); });


  server.on("/ataque_frontal", []() { ataqueFrontal(); server.send(200, "text/plain", "ok"); });
  server.on("/ataque_traseiro", []() { ataqueTraseiro(); server.send(200, "text/plain", "ok"); });
  server.on("/ataque_lateral", []() { ataqueLateral(); server.send(200, "text/plain", "ok"); });
  server.on("/defesa", []() { defesa(); server.send(200, "text/plain", "ok"); });

  server.begin();
}


void setup() {

  Serial.begin(115200);

 
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorIN3, OUTPUT);
  pinMode(motorIN4, OUTPUT);

  parar();


  servoFrontal.attach(pinFrontal);
  servoTraseiro.attach(pinTraseiro);
  servoLateral.attach(pinLateral);
  servoDefesa.attach(pinDefesa);

  servoFrontal.write(90);
  servoTraseiro.write(90);
  servoLateral.write(90);
  servoDefesa.write(90);


  Serial.println("Iniciando Access Point...");
  WiFi.softAP(ssid, password);

  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());

  
  setupServer();
}


void loop() {
  server.handleClient();
}
