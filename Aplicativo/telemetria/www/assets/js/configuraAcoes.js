var log = function(dados) {
    // if (!painel) {
    //     console.log(dados);
    //     return;
    // }
    $('#areaMensagens').append(dados + "\n");
    $('#areaMensagens').trigger('autoresize');
};

function toggleFullScreen() {
    if ((document.fullScreenElement && document.fullScreenElement !== null) ||
        (!document.mozFullScreen && !document.webkitIsFullScreen)) {
        if (document.documentElement.requestFullScreen) {
            document.documentElement.requestFullScreen();
        }
        else if (document.documentElement.mozRequestFullScreen) {
            document.documentElement.mozRequestFullScreen();
        }
        else if (document.documentElement.webkitRequestFullScreen) {
            document.documentElement.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
        }
    }
    else {
        if (document.cancelFullScreen) {
            document.cancelFullScreen();
        }
        else if (document.mozCancelFullScreen) {
            document.mozCancelFullScreen();
        }
        else if (document.webkitCancelFullScreen) {
            document.webkitCancelFullScreen();
        }
    }
}

$(document).ready(function() {

    $('#botaoGravar').click(function(e) {
        var texto = $("#arquivo").val();
        if (texto === "@") {
            $("#enviarComandos").show();
            $("#arquivo").val("");
            return;
        }else{
            send('2:'+texto);
        }
        $("#arquivo").val("");
    });

    $('#botaoEnviar').click(function(e) {
        send($('#enviar').val());
        // impede o envio do form
    });

    $("#botaoTelaCheia").click(toggleFullScreen);

    $("#botaoConectar").click(conecta);

    var oldLog = console.log;
    console.log = function(message) {
        log(message);
        oldLog.apply(console, arguments);
    };

    var oldErrorLog = console.error;
    console.error = function(message) {
        log(message);
        oldErrorLog.apply(console, arguments);
    };

    $("#enviarComandos").hide();
    //conecta();
});