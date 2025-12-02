

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>


const char* AC_SSID = "Tow Mater";
const char* AC_PWD  = "12345678";


const int in1 = 26;
const int in2 = 27;
const int in3 = 32;
const int in4 = 33;


Servo servoTeto;        
Servo servoFrenteDir;   
Servo servoFrenteEsq;   
Servo servoTrasDir;     
Servo servoTrasEsq;     

const int PIN_TETO       = 12;
const int PIN_FRENTE_DIR = 13;
const int PIN_FRENTE_ESQ = 14;
const int PIN_TRAS_DIR   = 25;
const int PIN_TRAS_ESQ   = 15;

WebServer server(80);


const char* PAGINA = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
<title>Tow Mater — Painel Enferrujado</title>

<style>
  @import url('https://fonts.googleapis.com/css2?family=Russo+One&display=swap');

  html,body{ height:100%; }
  body{
    margin:0;
    font-family:'Russo One', sans-serif;
    background: #2b1a12 url('https://i.ibb.co/4fH3CM5/rust-texture.jpg') center/cover fixed;
    -webkit-font-smoothing:antialiased;
    display:flex;
    align-items:center;
    justify-content:center;
    padding:16px;
    color:#ffd9a8;
  }

  .panel{
    width:420px;
    max-width:98%;
    background: linear-gradient(180deg, rgba(20,10,6,0.9), rgba(40,20,12,0.95));
    border:6px solid #5c2e12;
    border-radius:18px;
    padding:18px;
    box-shadow: 0 18px 40px rgba(0,0,0,0.7), inset 0 -6px 30px rgba(255,140,60,0.05);
    text-align:center;
  }

  .mater {
    width:240px;
    margin:0 auto 8px auto;
    display:block;
    filter: drop-shadow(0 6px 12px rgba(0,0,0,0.6));
  }

  h1{
    margin:0 0 10px 0;
    font-size:22px;
    color:#ffcf91;
    text-shadow: 0 3px 0 #6b3f20;
  }

  .section { margin-top:10px; }

  .moves {
    display:flex;
    gap:8px;
    justify-content:center;
    flex-wrap:wrap;
    margin-bottom:8px;
  }

  .bigBtn {
    min-width:110px;
    padding:12px 10px;
    border-radius:12px;
    border:0;
    font-weight:700;
    font-size:15px;
    color:#3b2a1a;
    background: linear-gradient(180deg,#ffd9a0,#ffb36a);
    box-shadow: 0 8px 14px rgba(0,0,0,0.25);
    cursor:pointer;
  }

  .servoGrid{
    display:grid;
    grid-template-columns: repeat(2,1fr);
    gap:10px;
    margin-top:10px;
  }

  .servoCard{
    background: linear-gradient(180deg, rgba(255,255,255,0.04), rgba(0,0,0,0.06));
    padding:8px;
    border-radius:10px;
    border: 2px solid rgba(255,180,80,0.06);
  }

  .label { font-size:13px; color:#ffdcae; margin-bottom:6px; }

  .btn {
    width:100%;
    padding:10px 6px;
    margin-top:6px;
    border-radius:10px;
    border:0;
    font-weight:700;
    font-size:14px;
    color:#fff;
    background: linear-gradient(180deg,#7a3e1a,#3a1d0b);
    box-shadow: 0 6px 10px rgba(0,0,0,0.4);
    cursor:pointer;
  }

  .duoRow { display:flex; gap:8px; justify-content:center; margin-top:10px; }

  footer { font-size:12px; color:#a07a58; margin-top:12px; }
  @media (max-width:460px){
    .mater{ width:180px; }
    .bigBtn{ min-width:90px; font-size:14px; padding:10px; }
    .panel{ padding:12px; }
  }
</style>
</head>
<body>
  <div class="panel">
    <!-- Tow Mater face image (B — front/face) -->
    <img class="mater" src="https://i.imgur.com/ew9u1dT.png" alt="Tow Mater">

    <h1>Tow Mater — Painel Enferrujado</h1>

    <div class="section">
      <div class="label">Movimento</div>
      <div class="moves">
        <button class="bigBtn" onclick="send('/frente')">⬆ Frente</button>
        <button class="bigBtn" onclick="send('/parar')">⏹ Parar</button>
        <button class="bigBtn" onclick="send('/tras')">⬇ Trás</button>
      </div>
      <div class="duoRow">
        <button class="bigBtn" onclick="send('/esquerda')">⬅ Esquerda</button>
        <button class="bigBtn" onclick="send('/direita')">➡ Direita</button>
      </div>
    </div>

    <div class="section">
      <div class="label">Servos Individuais</div>
      <div class="servoGrid">
        <div class="servoCard">
          <div class="label">Teto (S1)</div>
          <button class="btn" onclick="send('/teto_up')">↑ Abrir</button>
          <button class="btn" onclick="send('/teto_down')">↓ Fechar</button>
        </div>

        <div class="servoCard">
          <div class="label">Frente Dir (S2)</div>
          <button class="btn" onclick="send('/fdir_up')">↑</button>
          <button class="btn" onclick="send('/fdir_down')">↓</button>
        </div>

        <div class="servoCard">
          <div class="label">Frente Esq (S3)</div>
          <button class="btn" onclick="send('/fesq_up')">↑</button>
          <button class="btn" onclick="send('/fesq_down')">↓</button>
        </div>

        <div class="servoCard">
          <div class="label">Trás Dir (S4)</div>
          <button class="btn" onclick="send('/tdir_up')">↑</button>
          <button class="btn" onclick="send('/tdir_down')">↓</button>
        </div>

        <div class="servoCard" style="grid-column:1 / -1">
          <div class="label">Trás Esq (S5)</div>
          <button class="btn" onclick="send('/tesq_up')">↑</button>
          <button class="btn" onclick="send('/tesq_down')">↓</button>
        </div>
      </div>
    </div>

    <div class="section">
      <div class="label">Servos Duplos</div>
      <div class="duoRow">
        <button class="btn" onclick="send('/frente_up')">Frente ↑ (S2+S3)</button>
        <button class="btn" onclick="send('/frente_down')">Frente ↓ (S2+S3)</button>
      </div>
      <div class="duoRow">
        <button class="btn" onclick="send('/tras_up')">Trás ↑ (S4+S5)</button>
        <button class="btn" onclick="send('/tras_down')">Trás ↓ (S4+S5)</button>
      </div>
    </div>

    <footer>Design inspirado no Tow Mater — não oficial</footer>
  </div>

<script>
function send(path){
  fetch(path).catch(()=>{ /* silêncio se falhar */ });
}
</script>
</body>
</html>
)rawliteral";

void motorFrente(){ digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); server.send(200, "text/html", PAGINA); }
void motorTras()  { digitalWrite(in1, LOW);  digitalWrite(in2, HIGH); digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); server.send(200, "text/html", PAGINA); }
void motorEsquerda(){ digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); server.send(200, "text/html", PAGINA); }
void motorDireita() { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); server.send(200, "text/html", PAGINA); }
void motorParar()   { digitalWrite(in1, LOW);  digitalWrite(in2, LOW);  digitalWrite(in3, LOW);  digitalWrite(in4, LOW);  server.send(200, "text/html", PAGINA); }


void teto_up(){ servoTeto.write(150); server.send(200,"text/plain","OK"); }
void teto_down(){ servoTeto.write(30); server.send(200,"text/plain","OK"); }

void fdir_up(){ servoFrenteDir.write(150); server.send(200,"text/plain","OK"); }
void fdir_down(){ servoFrenteDir.write(30); server.send(200,"text/plain","OK"); }

void fesq_up(){ servoFrenteEsq.write(150); server.send(200,"text/plain","OK"); }
void fesq_down(){ servoFrenteEsq.write(30); server.send(200,"text/plain","OK"); }

void tdir_up(){ servoTrasDir.write(150); server.send(200,"text/plain","OK"); }
void tdir_down(){ servoTrasDir.write(30); server.send(200,"text/plain","OK"); }

void tesq_up(){ servoTrasEsq.write(150); server.send(200,"text/plain","OK"); }
void tesq_down(){ servoTrasEsq.write(30); server.send(200,"text/plain","OK"); }


void frente_up(){ servoFrenteDir.write(150); servoFrenteEsq.write(150); server.send(200,"text/plain","OK"); }
void frente_down(){ servoFrenteDir.write(30); servoFrenteEsq.write(30); server.send(200,"text/plain","OK"); }

void tras_up(){ servoTrasDir.write(150); servoTrasEsq.write(150); server.send(200,"text/plain","OK"); }
void tras_down(){ servoTrasDir.write(30); servoTrasEsq.write(30); server.send(200,"text/plain","OK"); }


void setup(){
  Serial.begin(115200);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  
  servoTeto.attach(PIN_TETO);
  servoFrenteDir.attach(PIN_FRENTE_DIR);
  servoFrenteEsq.attach(PIN_FRENTE_ESQ);
  servoTrasDir.attach(PIN_TRAS_DIR);
  servoTrasEsq.attach(PIN_TRAS_ESQ);

  
  servoTeto.write(90);
  servoFrenteDir.write(90);
  servoFrenteEsq.write(90);
  servoTrasDir.write(90);
  servoTrasEsq.write(90);

  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AC_SSID, AC_PWD);
  Serial.println();
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  
  server.on("/", [](){ server.send(200, "text/html", PAGINA); });

  
  server.on("/frente", motorFrente);
  server.on("/tras", motorTras);
  server.on("/esquerda", motorEsquerda);
  server.on("/direita", motorDireita);
  server.on("/parar", motorParar);

  
  server.on("/teto_up", teto_up);
  server.on("/teto_down", teto_down);

  server.on("/fdir_up", fdir_up);
  server.on("/fdir_down", fdir_down);

  server.on("/fesq_up", fesq_up);
  server.on("/fesq_down", fesq_down);

  server.on("/tdir_up", tdir_up);
  server.on("/tdir_down", tdir_down);

  server.on("/tesq_up", tesq_up);
  server.on("/tesq_down", tesq_down);

  
  server.on("/frente_up", frente_up);
  server.on("/frente_down", frente_down);

  server.on("/tras_up", tras_up);
  server.on("/tras_down", tras_down);

  server.begin();
}


void loop(){
  server.handleClient();
}
