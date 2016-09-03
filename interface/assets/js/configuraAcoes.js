var log = function(dados) {
    // if (!painel) {
    //     console.log(dados);
    //     return;
    // }
    $('#areaMensagens').append(dados + "\n");
    $('#areaMensagens').trigger('autoresize');
};

$(document).ready(function() {
    
    $('#formEnviar').submit(function(e) {
        // impede o envio do form
        e.preventDefault();
        send($('#enviar').value);
    });

    $('#formArquivo').submit( function(e) {
        send('{\"cod\":2,\"arq\":' + $('#arquivo').value + '}');
        // impede o envio do form
        e.preventDefault();
    });


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
});
