var consumoInstantaneo = document.getElementById('consumoInstantaneo');
var autonomia = document.getElementById('autonomia');
var velocidade;

var raio = 0.295; // em metros
var pulsosPorVolta = 4;

var totalPulsos = 0;


var conectando = 0;
var botao = document.getElementById("botao");
var ws;

// function conecta() {
//     conectando = 1;
//     ws = new WebSocket('ws://192.168.4.1:81');
//     ws.onopen = function(e) {
//         log('Conectado', true);
//         botao.style.visibility = "hidden";
//     };
//     ws.onclose = function(e) {
//         log('Desconectado', true);
//         conectando = 0;
//         botao.style.visibility = "visible";
//         botao.onclick(function() {
//             if (conectando === 0)
//                 conecta();
//         });
//         botao.innerHTML = "Conectar";
//         /*
//          setTimeout(function () {
//          conecta();
//          }, 5);
//          */
//     };
//     ws.onerror = function(e) {
//         log('Erro na conexão!', true);
//         conectando = 0;
//         botao.style.visibility = "visible";
//         botao.onclick(function() {
//             if (conectando === 0)
//                 conecta();
//         });
//         botao.innerHTML = "Conectar";
//         /*
//          setTimeout(function () {
//          conecta();
//          }, 5);
//          */
//     };
//     ws.onmessage = function(e) {
//         trataDadosRecebidos(e);
//     };
// }

// function send(msg) {
//     waitForSocketConnection(function() {
//         msg += ';';
//         ws.send(msg);
//         log('Message sent:' + msg);
//     });
// }

// function trataDadosRecebidos(e) {
//     //log('Recebido: ' + e.data.substring(0, e.data.length - 1));
//     try {
//         dados = JSON.parse(e.data.substring(0, e.data.length - 1));
//         if (dados['01'] != null) {
//             tensao = parseFloat(dados['01'])
//             voltimetro.set(tensao);
//         }
//         if (dados['02'] != null) {
//             corrente = parseFloat(dados['02']);
//             amperimetro.set(corrente);
//         }
//         if (dados['03'] != null) {
//             var pulsos = parseInt(dados['03']);
//             var circunferencia = 2.0 * 3.1415 * raio;
//             velocidade = (pulsos * 3.6 * circunferencia) / (pulsosPorVolta);
//             velocimetro.set(velocidade);
//             totalPulsos += pulsos;
//             document.getElementById('distancia').value = totalPulsos;
//         }
//         if (dados['04'] != null) {
//             temp = parseFloat(dados['04']);
//             termometro.set(temp);
//         }
//         if (dados['20'] != null) {
//             log(dados['20'], true);
//         }
//         //Calcula potencia instantânea
//         if (!isNaN(tensao) && !isNaN(corrente)) {
//             wattimetro.set(tensao * corrente);
//             //calcula consumo instantâneo
//             if (!isNaN(velocidade)) {
//                 consumoInstantaneo.innerHTML = (tensao * corrente / velocidade);
//             }
//         }
//     }
//     catch (err) {
//         log(e.data);
//         console.log(err.message);
//         return;
//     }
// }



// function waitForSocketConnection(callback) {
//     setTimeout(
//         function() {
//             if (ws.readyState === 1) {
//                 if (callback !== undefined) {
//                     callback();
//                 }
//                 return;
//             }
//             else {
//                 waitForSocketConnection(callback);
//             }
//         }, 5);
// }

// window.onload = function(e) {
//     //botao.style.visibility = "hidden";
//     conecta();
// };
