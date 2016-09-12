#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#define DEBUG
const char *ssid = "TelemetriaNova";
const char *password = "12345678";

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char saida[128];

WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t numClient = -1;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      numClient = -1;
#ifdef DEBUG
      Serial.printf("[%u] Disconnected!\n", num);
#endif
      break;

    case WStype_CONNECTED: {
        numClient = num;
#ifdef DEBUG
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif
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
  if (numClient == -1) return;
  size_t tam = inputString.length();
  inputString.toCharArray(saida, tam + 1);

#ifdef DEBUG
  Serial.println(inputString);
  Serial.println(saida);
#endif

  webSocket.sendTXT(numClient, saida, tam);
  inputString = "";
}

void setup() {
  Serial.begin(9600);
#ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.println();
#endif
  inputString.reserve(128);

  WiFi.softAP(ssid, password, 1, 0);
  for (uint8_t t = 4; t > 0; t--) {
#ifdef DEBUG
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
#endif
    delay(1000);
  }

#ifdef DEBUG
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
#endif

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

#ifdef DEBUG
unsigned long previousMillis = 0;
const long interval = 100;
#endif

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

#ifdef DEBUG
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    inputString = "";
    inputString = "{\"01\":\"";
    inputString += String(map(analogRead(A0), 0, 1024, 0, 50));
    inputString += "\"}";
    envia();
  }
#endif
}

