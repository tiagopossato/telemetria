$(document).ready(function() {
    // the "href" attribute of .modal-trigger must specify the modal ID that wants to be triggered
    $('.modalAcoes').leanModal();
});


var log = function(dados, painel) {
    if (!painel) {
        console.log(dados);
        return;
    }

    $('#areaMensagens').append(dados+"\n");
    $('#areaMensagens').trigger('autoresize');

    document.getElementById("areaMensagens").scrollTop = document.getElementById("areaMensagens").scrollHeight;
};

function send(msg) {
    waitForSocketConnection(function() {
        msg += ';';
        if (ws) ws.send(msg);
        log('Message sent:' + msg);
    });
}
