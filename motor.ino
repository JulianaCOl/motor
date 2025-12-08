const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<title>Tow Mater Control Panel</title>

<style>
body {
    margin: 0;
    padding: 0;
    font-family: "Russo One", sans-serif;
    background: url('https://i.imgur.com/8gsqK9V.jpeg');
    background-size: cover;
    background-position: center;
    color: #fff;
    text-align: center;
}

/* Painel enferrujado */
.panel {
    background: url('https://i.imgur.com/R3bw8CX.jpeg');
    background-size: cover;
    border: 6px solid #653B22;
    border-radius: 20px;
    width: 90%;
    margin: auto;
    padding: 20px;
    margin-top: 20px;
    box-shadow: 0px 0px 25px #000;
}

/* Logo Tow Mater */
.logo {
    width: 260px;
    margin-bottom: 10px;
    filter: drop-shadow(0px 0px 6px #000);
}

/* Botões metálicos */
button {
    width: 150px;
    height: 70px;
    margin: 12px;
    font-size: 24px;
    border-radius: 12px;
    border: 3px solid #3b2a21;
    background: linear-gradient(145deg, #5d4535, #3b2a21);
    color: #fff;
    text-shadow: 2px 2px 2px #000;
    box-shadow: inset 2px 2px 4px #2d1e15, 2px 2px 6px #000;
    transition: 0.2s;
}

button:active {
    transform: scale(0.94);
    box-shadow: inset 2px 2px 8px #000;
}

#direcao {
    margin-top: 20px;
}

h1, h2 {
    text-shadow: 3px 3px 2px #000;
    color: #e0d6c2;
}

@media (max-width: 600px) {
    button {
        width: 42%;
        height: 65px;
        font-size: 22px;
    }
}
</style>

</head>
<body>

<img class="logo" src="https://i.imgur.com/c0wM6rU.png">

<div class="panel">

<h1>Tow Mater Control Panel</h1>

<div id="direcao">
    <button onclick="send('frente')">⬆ Frente</button><br>
    <button onclick="send('esquerda')">⬅ Esquerda</button>
    <button onclick="send('parar')">⛔ Parar</button>
    <button onclick="send('direita')">➡ Direita</button><br>
    <button onclick="send('tras')">⬇ Trás</button>
</div>

<h2>Ataques</h2>
<button onclick="send('ataque_frontal')">⚡ Frontal</button>
button onclick="send('ataque_traseiro')">💥 Traseiro</button><br>
<button onclick="send('defesa')">🛡 Defesa</button>
<button onclick="send('combo')">🔥 Combo</button>

</div>

<script>
function send(cmd) {
    fetch('/' + cmd);
}
</script>

</body>
</html>
)=====";
