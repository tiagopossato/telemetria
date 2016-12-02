var ws;

var tensao = 0;
var corrente = 0;
var velocidade = 0;

function conecta() {
    try {
        ws = new WebSocket('ws://192.168.4.1:81');
    }
    catch (err) {
        console.log(err);
    }
    $("#botaoConectar").hide('slow');
    log('Conectando...', true);

    ws.onopen = function(e) {
        log('Conectado', true);
        $("#botaoConectar").hide('slow');
	send('1:175');
    };

    ws.onclose = function(e) {
        log('Desconectado', true);
        $("#botaoConectar").show('slow');
    };

    ws.onerror = function(e) {
        log('Erro na conexão!', true);
        $("#botaoConectar").show('slow');
    };
    ws.onmessage = function(e) {
        trataDadosRecebidos(e);
    };
}

function send(msg) {
    waitForSocketConnection(function() {
        msg += ';';
        ws.send(msg);
        log('Mensagem enviada:' + msg);
    });
}

function trataDadosRecebidos(e) {
    //log('Recebido: ' + e.data);
    try {
        var dados = JSON.parse(e.data);
        if (dados['01'] != null) {
            tensao = parseFloat(dados['01'])
            medidorTensao.refresh(tensao);
        }
        if (dados['02'] != null) {
            corrente = parseFloat(dados['02']);
            medidorCorrente.refresh(corrente);
        }
        if (dados['03'] != null) {
            velocidade = parseInt(dados['03']);
            velocimetro.refresh(velocidade);
        }
        if (dados['04'] != null) {
            var distancia = parseFloat(dados['04']);
            hodometro.refresh(distancia);
        }

        if (dados['05'] != null) {
            var temp = parseFloat(dados['05']);
            temperaturaBaterias.refresh(temp);
        }

        if (dados['06'] != null) {
            var temp = parseFloat(dados['06']);
            temperaturaCockpit.refresh(temp);
        }

        if (dados['20'] != null) {
            log(dados['20'], true);
        }

        //Calcula potencia instantânea
        if (!isNaN(tensao) && tensao > 0.1 && !isNaN(corrente) && corrente > 0.01) {
            wattimetro.refresh(tensao * corrente);
            //calcula consumo instantâneo
            if (!isNaN(velocidade) && velocidade > 0) {
                consumoInstantaneo.refresh(tensao * corrente / velocidade);
		autonomiaInstantanea.refresh(velocidade / (tensao * corrente));
            }
        }
    }
    catch (err) {
        log(e.data);
        console.error(err.message);
        return;
    }
}

function waitForSocketConnection(callback) {
    setTimeout(
        function() {
            if (ws.readyState === 1) {
                if (callback !== undefined) {
                    callback();
                }
                return;
            }
            else {
                waitForSocketConnection(callback);
            }
        }, 5);
}
