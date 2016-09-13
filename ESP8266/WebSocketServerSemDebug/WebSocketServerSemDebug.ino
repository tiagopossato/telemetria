#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>

const char *ssid = "carro";
const char *password = "";

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char saida[128];

WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t numClient = 255;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      numClient = 255;
      break;
    case WStype_CONNECTED: {
        numClient = num;
        // send message to client
        webSocket.sendTXT(num, "{\"20\":\"Connected\"}");
      }
      break;
    case WStype_TEXT:
      Serial.printf("%s\n", payload);
      break;
  }
}

void envia() {
//  if (numClient == 255) {
//    inputString = "";
//    return;
//  }
  size_t tam = inputString.length();
  inputString.toCharArray(saida, tam + 1);
  webSocket.sendTXT(numClient, saida, tam);
  inputString = "";
}

void setup() {
  Serial.begin(9600);
  inputString.reserve(128);

  /**
     Set up an access point
     @param ssid          Pointer to the SSID (max 63 char).
     @param passphrase    (for WPA2 min 8 char, for open use NULL)
     @param channel       WiFi channel number, 1 - 13.
     @param ssid_hidden   Network cloaking (0 = broadcast SSID, 1 = hide SSID)
  */
  WiFi.softAP(ssid, password, 14, 0);

  for (uint8_t t = 3; t > 0; t--) {
    delay(1000);
  }

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}


void loop() {
  webSocket.loop();
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      envia();
    } else {
      inputString += inChar;
    }
  }
}

