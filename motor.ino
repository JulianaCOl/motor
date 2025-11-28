#include <WiFi.h>
#include <WebServer.h>

const char* AC_SSID = "Tow Mater";
const char* AC_PWD = "12345678";


const int in1 = 26;
const int in2 = 27;
const int in3 = 32;
const int in4 = 33;

WebServer server(80);


const char* PAGINA = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Tow Mater Controls</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Russo+One&display=swap');

    body {
      background: radial-gradient(circle at center, #3b2416, #1a0d07);
      font-family: 'Russo One', sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      color: #f5e0b3;
      margin: 0;
    }

    .painel {
      background: linear-gradient(145deg, #4b2e1e, #2b1a12);
      border: 5px solid #7b4a26;
      border-radius: 20px;
      padding: 25px;
      width: 360px;
      max-width: 95%;
      text-align: center;
      box-shadow: 0 0 25px rgba(0, 0, 0, 0.8),
                  inset 0 0 25px rgba(255, 180, 80, 0.1);
      position: relative;
    }

    header h1 {
      color: #ffb84d;
      text-shadow: 0 0 12px #ff8000;
      font-size: 1.3rem;
    }

    .controle {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 15px;
    }

    .linha-meio {
      display: flex;
      gap: 15px;
    }

    .botao {
      width: 90px;
      height: 90px;
      font-size: 2rem;
      border-radius: 15px;
      border: 3px solid #f4d2a1;
      background: linear-gradient(145deg, #5a3c2b, #2b1b10);
      color: #ffe8c0;
      box-shadow: 0 8px 0 #1a100a, 0 0 15px rgba(0, 0, 0, 0.7);
      transition: 0.1s;
    }

    .botao:active {
      transform: translateY(4px);
      box-shadow: 0 3px 0 #1a100a;
    }

    .frente { border-color: #7fff7f; }
    .re { border-color: #ff4c4c; }
    .esquerda { border-color: #ffaa00; }
    .direita { border-color: #66ccff; }
    .parar { border-color: #ffdd55; }

    footer {
      margin-top: 20px;
      font-size: 0.8rem;
      color: #e8d3b2;
    }
  </style>
</head>
<body>
  <div class="painel">
    <header>
      <h1>⚙️ Tow Mater — Controle ⚙️</h1>
      <p>Controle seu carro remotamente!</p>
    </header>

    <div class="controle">
      <button class="botao frente" onclick="enviar('/frente')">⬆</button>

      <div class="linha-meio">
        <button class="botao esquerda" onclick="enviar('/esquerda')">⬅</button>
        <button class="botao parar" onclick="enviar('/parar')">⏹</button>
        <button class="botao direita" onclick="enviar('/direita')">➡</button>
      </div>

      <button class="botao re" onclick="enviar('/re')">⬇</button>
    </div>

    <footer>Tow Mater • Powered by ESP32</footer>
  </div>

  <script>
    function enviar(cmd) {
      fetch(cmd);
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() { server.send(200, "text/html", PAGINA); }

void motorFrente() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  server.send(200, "text/html", PAGINA);
}

void motorRe() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  server.send(200, "text/html", PAGINA);
}

void motorEsquerda() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  server.send(200, "text/html", PAGINA);
}

void motorDireita() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  server.send(200, "text/html", PAGINA);
}

void motorParar() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  server.send(200, "text/html", PAGINA);
}

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AC_SSID, AC_PWD);

  Serial.println("Rede WiFi Tow Mater criada!");
  Serial.print("Acesse em: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/frente", motorFrente);
  server.on("/re", motorRe);
  server.on("/esquerda", motorEsquerda);
  server.on("/direita", motorDireita);
  server.on("/parar", motorParar);

  server.begin();
}

void loop() {
  server.handleClient();
}
