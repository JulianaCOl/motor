// Sensibilidade do efeito
const intensidade = 25;

function atualizarFundo(beta, gamma) {
    // beta  = inclinação frente/trás
    // gamma = inclinação esquerda/direita

    let x = gamma * intensidade;
    let y = beta * intensidade;

    document.body.style.backgroundPosition = `${50 + x}% ${50 + y}%`;
}

// ANDROID → funciona direto
window.addEventListener("deviceorientation", (event) => {
    atualizarFundo(event.beta, event.gamma);
});

// iPHONE → precisa pedir permissão
if (typeof DeviceOrientationEvent !== "undefined" &&
    typeof DeviceOrientationEvent.requestPermission === "function") {

    document.body.addEventListener("click", () => {
        DeviceOrientationEvent.requestPermission().then((resp) => {
            if (resp === "granted") {
                window.addEventListener("deviceorientation", (event) => {
                    atualizarFundo(event.beta, event.gamma);
                });
            }
        });
    });
}
