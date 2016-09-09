#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char saida[512];

WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = "TelemetriaNova";
const char *password = "12345678";
uint8_t numClient;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Cliente desconectado!");
      break;
    case WStype_CONNECTED:
      {
        numClient = num;
        IPAddress ip = webSocket.remoteIP(num);
        Serial.println("Cliente Conectado: " + ip);
      }
      break;
    case WStype_TEXT:
      {
        String text = String((char *) &payload[0]);
        Serial.println(text);
      }
      break;
    default:
      break;
  }

}

void setup() {
  Serial.begin(9600);
  inputString.reserve(512);
  WiFi.softAP(ssid, password, 14, 0);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      size_t tam = inputString.length();
      inputString.toCharArray(saida, tam);
      webSocket.sendTXT(numClient, saida, tam);
      // clear the string:
      inputString = "";
    } else {
      // add it to the inputString:
      inputString += inChar;
    }
  }
}

