

function send(msg) {
    waitForSocketConnection(function() {
        msg += ';';
        if (ws) ws.send(msg);
        log('Message sent:' + msg);
    });
}
