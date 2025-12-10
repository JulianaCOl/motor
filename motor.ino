#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ---------- WIFI ----------
const char* STA_SSID = "Tplink";
const char* STA_PWD  = "senac1234";

// const char* STA_SSID = "CONECTA_SENAC";
// const char* STA_PWD  = "";

WebServer server(80);

// ---------- MOTORES ----------
const int IN1 = 26;
const int IN2 = 27;
const int IN3 = 32;
const int IN4 = 33;

const int ENA = 25;
const int ENB = 14;

const int PWM_CH_A = 0;
const int PWM_CH_B = 1;
const int PWM_FREQ = 5000;
const int PWM_RES  = 8;

int velocidade = 200;

// ---------- SERVOS ----------
Servo servoFrontal1;
Servo servoFrontal2;
Servo servoTraseiro1;
Servo servoTraseiro2;
Servo servoTeto;

#define PIN_FRONTAL1 12
#define PIN_FRONTAL2 13
#define PIN_TRASEIRO1 15
#define PIN_TRASEIRO2 2
#define PIN_TETO 4

// ---------- MOVIMENTOS ----------
void frente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(PWM_CH_A, velocidade);
  ledcWrite(PWM_CH_B, velocidade);
}

void tras() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CH_A, velocidade);
  ledcWrite(PWM_CH_B, velocidade);
}

void esquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CH_A, velocidade);
  ledcWrite(PWM_CH_B, velocidade);
}

void direita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(PWM_CH_A, velocidade);
  ledcWrite(PWM_CH_B, velocidade);
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CH_A, 0);
  ledcWrite(PWM_CH_B, 0);
}

// ---------- ATAQUES ----------
void ataqueFrontal1(){ servoFrontal1.write(150); delay(300); servoFrontal1.write(90); }
void ataqueFrontal2(){ servoFrontal2.write(30);  delay(300); servoFrontal2.write(90); }
void ataqueTraseiro1(){ servoTraseiro1.write(150); delay(300); servoTraseiro1.write(90); }
void ataqueTraseiro2(){ servoTraseiro2.write(30);  delay(300); servoTraseiro2.write(90); }
void ataqueTeto(){ servoTeto.write(160); delay(400); servoTeto.write(90); }
void ataqueCombo(){ ataqueFrontal1(); ataqueTraseiro1(); }

// ---------- HTML ----------
String htmlPage() {
return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>
* { margin:0; padding:0; box-sizing:border-box; }

body {
  width:100vw;
  height:100vh;
  overflow:hidden;

  /* 🔥 Ajuste correto da imagem Tow Mater */
  background-image:url('https://i.pinimg.com/736x/ef/84/53/ef8453c302d3456c94e52c26912ec37e.jpg');
  background-size:cover;       /* cobre a tela SEM bordas */
  background-position:center;  /* centraliza */
  background-repeat:no-repeat; /* sem repetição */
  
  font-family:Arial;
}

.painel {
  width:100%;
  height:100%;
  display:flex;
  justify-content:space-between;
  align-items:center;
  padding:6px;
}

/* Direção */
.grupo-direcao { 
  display:flex;
  flex-direction:column;
  gap:10px;
  align-items:center;
}

/* Botões de direção */
.btn-direction {
  width:60px;
  height:60px;
  font-size:26px;
  border-radius:50%;
  border:2px solid #222;
  background:#d9d9d9;
  box-shadow:0 0 6px #0008;
  font-weight:bold;
}

/* Painel de ataques */
.grupo-ataque {
  display:flex;
  flex-direction:column;
  gap:8px;
}

/* Botões enferrujados com bordas irregulares */
.btn-ataque {
  width:170px;
  padding:10px 12px;
  font-size:13px;
  font-weight:bold;
  color:#fff;

  background:#7a4b25;
  border:3px solid #4a2d18;

  clip-path: polygon(
    4% 0%, 93% 3%, 100% 18%, 
    98% 82%, 90% 100%, 8% 96%, 
    0% 75%, 2% 20%
  );

  box-shadow:
    inset 0px 0px 8px #2a1a0f,
    0 0 6px #000;
}

h1 {
  width:100%;
  text-align:center;
  font-size:24px;
  padding-top:4px;
  color:white;
  text-shadow:2px 2px 6px #000;
}
</style>
</head>

<body>

<h1>Tow Mater</h1>

<div class="painel">

  <!-- Direção -->
  <div class="grupo-direcao">
    <button class="btn-direction" onclick="fetch('/frente')">▲</button>

    <div style="display:flex; gap:10px;">
      <button class="btn-direction" onclick="fetch('/esquerda')">◄</button>
      <button class="btn-direction" onclick="fetch('/parar')">■</button>
      <button class="btn-direction" onclick="fetch('/direita')">►</button>
    </div>

    <button class="btn-direction" onclick="fetch('/tras')">▼</button>
  </div>

  <!-- Ataques -->
  <div class="grupo-ataque">
    <button class="btn-ataque" onclick="fetch('/af1')">ATAQUE FRONTAL 1</button>
    <button class="btn-ataque" onclick="fetch('/af2')">ATAQUE FRONTAL 2</button>
    <button class="btn-ataque" onclick="fetch('/at1')">ATAQUE TRASEIRO 1</button>
    <button class="btn-ataque" onclick="fetch('/at2')">ATAQUE TRASEIRO 2</button>
    <button class="btn-ataque" onclick="fetch('/teto')">ATAQUE TETO</button>
    <button class="btn-ataque" onclick="fetch('/combo')">COMBO FRENTE + TRÁS</button>
  </div>

</div>
</body>
</html>
)rawliteral";
}

// ---------- SETUP / LOOP ----------
void setup(){
  Serial.begin(115200);

  // IP fixo
// IPAddress local_IP(192, 168, 16, 55);
// IPAddress gateway(192, 168, 16, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(8, 8, 8, 8);

//   WiFi.config(local_IP, gateway, subnet, dns);

    WiFi.mode(WIFI_STA);
    WiFi.begin(STA_SSID, STA_PWD);

   Serial.print("Conectando ao WiFi ");
    Serial.println(STA_SSID);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
        delay(500);
        Serial.print(".");
        tentativas++;
    }

  
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nNAO CONECTOU AO WIFI :(");
        return; // não continua se não tiver WiFi
    }

    Serial.println("\nConectado!");
    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Mascara: ");
    Serial.println(WiFi.subnetMask());

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  ledcSetup(PWM_CH_A,PWM_FREQ,PWM_RES);
  ledcSetup(PWM_CH_B,PWM_FREQ,PWM_RES);
  ledcAttachPin(ENA,PWM_CH_A);
  ledcAttachPin(ENB,PWM_CH_B);

  servoFrontal1.attach(PIN_FRONTAL1);
  servoFrontal2.attach(PIN_FRONTAL2);
  servoTraseiro1.attach(PIN_TRASEIRO1);
  servoTraseiro2.attach(PIN_TRASEIRO2);
  servoTeto.attach(PIN_TETO);

  servoFrontal1.write(90);
  servoFrontal2.write(90);
  servoTraseiro1.write(90);
  servoTraseiro2.write(90);
  servoTeto.write(90);

  server.on("/",[](){ server.send(200,"text/html",htmlPage()); });

  server.on("/frente",[](){ frente(); server.send(200,"text/plain","ok"); });
  server.on("/tras",[](){ tras(); server.send(200,"text/plain","ok"); });
  server.on("/esquerda",[](){ esquerda(); server.send(200,"text/plain","ok"); });
  server.on("/direita",[](){ direita(); server.send(200,"text/plain","ok"); });
  server.on("/parar",[](){ parar(); server.send(200,"text/plain","ok"); });

  server.on("/af1",[](){ ataqueFrontal1(); server.send(200,"text/plain","ok"); });
  server.on("/af2",[](){ ataqueFrontal2(); server.send(200,"text/plain","ok"); });
  server.on("/at1",[](){ ataqueTraseiro1(); server.send(200,"text/plain","ok"); });
  server.on("/at2",[](){ ataqueTraseiro2(); server.send(200,"text/plain","ok"); });
  server.on("/teto",[](){ ataqueTeto(); server.send(200,"text/plain","ok"); });
  server.on("/combo",[](){ ataqueCombo(); server.send(200,"text/plain","ok"); });

  server.begin();
}

void loop(){
  server.handleClient();
}
