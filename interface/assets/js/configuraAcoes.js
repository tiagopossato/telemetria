(function() {
    var oldLog = console.log;
    console.log = function(message) {
        log(message, true);
        oldLog.apply(console, arguments);
    };
})();

(function() {
    var oldErrorLog = console.error;
    console.error = function(message) {
        log(message, true);
        oldErrorLog.apply(console, arguments);
    };
})();

var log = function(dados, painel) {
    if (!painel) {
        console.log(dados);
        return;
    }
    $("#areaMensagens").value += dados + "\n";
    $("#areaMensagens").scrollTop = $("#areaMensagens").scrollHeight;
};

$('#formEnviar').submit(function(e) {
        // impede o envio do form
    e.preventDefault();
    send($('#enviar').value);
});

$('#formArquivo').submit('submit', function(e) {
    send('{\"cod\":2,\"arq\":' + $('#arquivo').value + '}');
    // impede o envio do form
    e.preventDefault();
});