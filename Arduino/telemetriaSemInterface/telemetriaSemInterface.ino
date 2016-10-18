#include <SPI.h>
#include <SD.h>
#include <TimerOne.h>
#include "telemetria.h"

/*
   quantidade de amostras na porta analogica
*/
#define AMOSTRAS  100

const byte pinoTensao = A0;
const byte pinoCorrente = A1;
const byte pinoTemperaturaBaterias = A3;
const byte pinoTemperaturaCockpit = A2;
const byte pinoRele = 2;
const byte pinoVelocidade = 3;
long pulsos = 0;


/*
   Intervalo do timer de medicao de velocidade
   em microsegundos
*/
const long int intervaloMedicaoVelocidade = 1000000;


String arquivo;//nome do arquivo

long intervaloEnvio;

boolean salvarDados = false;

void setup() {
  cfg.intervaloLeitura = 100;

  Serial.begin(9600);
  pinMode(pinoVelocidade, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinoVelocidade), incrementaPulso, FALLING); //FALLING for when the pin goes from high to low.

  arquivo.reserve(12);
  arquivo = String("medicoes.csv");

  Timer1.initialize(intervaloMedicaoVelocidade); // em microsegundos
  Timer1.attachInterrupt( contarPulsos ); // attach the service routine here

  pinMode(pinoRele, OUTPUT);
  // see if the card is present and can be initialized:
  abreCartao(true);
  chaveiaRele(true);

  // initialize the LED pin as an output:
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  static unsigned long previousMillisLeitura = 0;
  static unsigned long previousMillisExibicao = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisLeitura >= cfg.intervaloLeitura) {
    previousMillisLeitura = currentMillis;
    lerSensores();
    if (salvarDados) gravaDados();
  }

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (!digitalRead(buttonPin)) {
    if (salvarDados) {
      salvarDados = false;
      digitalWrite(ledVerde, LOW);
      enviar("20", "Parando...");
    }
    else {
      salvarDados = true;
      enviar("20", "Gravando...");
      // turn LED on:
      digitalWrite(ledVerde, HIGH);
    }
  }

  //filtro digital
  while (!digitalRead(buttonPin))delay(10);
}

void lerSensores() {
  long lido = 0;
  //tensao
  for (int i = 0; i < AMOSTRAS; i++) lido += analogRead(pinoTensao);
  //converte e aplica curva de calibração no valor lido
  dados.tensao = 0.96992571774 * converte((float) lido / AMOSTRAS, 0.0, 1023.0, 0.0, 60.0) + 0.3008745324;
  //corrente
  lido = 0;
  for (int i = 0; i < AMOSTRAS; i++) lido += analogRead(pinoCorrente);
  //converte e aplica curva de calibração
  dados.corrente = converte((float) lido / AMOSTRAS, 0.0, 1023.0, -30.0, 30.0) * 1.25;

  //temperatura
  lido = 0;
  for (int i = 0; i < AMOSTRAS; i++) lido += analogRead(pinoTemperaturaBaterias);
  dados.temperaturaBaterias = (((float) lido / AMOSTRAS) * 5.0 / 1023.0) / 0.01;

  lido = 0;
  for (int i = 0; i < AMOSTRAS; i++) lido += analogRead(pinoTemperaturaCockpit);
  dados.temperaturaCockpit = (((float) lido / AMOSTRAS) * 5.0 / 1023.0) / 0.01;
}

void gravaDados() {
  static int contador = 0;
  if (!abreCartao(false)) {
    if (!abreCartao(true)) {
      salvarDados = false;
      digitalWrite(ledVerde, LOW);
      return;
    }
  }

  File dataFile = SD.open(arquivo, FILE_WRITE);
  if (dataFile) {
    String dataString = String(dados.tensao) +
                        ";" + String(dados.corrente) +
                        ";" + String(dados.pulsosPorSegundo) +
                        ";" + String(dados.temperaturaBaterias) +
                        ";" + String(dados.temperaturaCockpit);
    dataFile.println(dataString);
    dataFile.close();
  } else {
    contador++;
    if (contador % 10 == 1)
      enviar("20", "Erro ao abrir o arquivo!");
    abreCartao(true);
  }
  if (contador >= 50) {
    contador = 0;
    salvarDados = false;
    digitalWrite(ledVerde, LOW);
    enviar("20", "Nao consegui abrir o arquivo, parando de gravar!");
  }
}

void incrementaPulso() {
  pulsos++;
}

void contarPulsos() {
  noInterrupts();
  dados.pulsosPorSegundo = pulsos;
  pulsos = 0;
  interrupts();
}

void chaveiaRele(bool estado) {
  static boolean rele = false;
  digitalWrite(pinoRele, estado);
  if (rele != estado) {
    rele = estado;
    if (rele) enviar("20", "Rele ligado");
    else enviar("20", "Rele desligado");
  }
}
