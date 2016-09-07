#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <TimerOne.h>
#include "telemetria.h"

/*
   DIFERENCA DE TEMPERATURA PARA ENVIAR PELA SERIAL
*/
#define STEP_TEMPERATURA 0.5
/*
   Intervalo para rodar a rotina de verificacao da tensao e atuar no rele
   Em milisegundos
*/
#define TEMPO_VERIFICACAO_RELE  500
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


String inputCodigo = "";
String arquivo;//nome do arquivo

long intervaloEnvio;

byte codigo = 0;
boolean salvarDados = false;
boolean enviarDados = false;

void setup() {
  cfg.raioRoda = 0.34;
  cfg.qtdPulsos = 4;
  cfg.intervaloLeitura = 100;

  Serial.begin(9600);
  pinMode(pinoVelocidade, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinoVelocidade), incrementaPulso, FALLING); //FALLING for when the pin goes from high to low.

  inputCodigo.reserve(50);
  arquivo.reserve(12);
  arquivo = String("medicoes.csv");

  Timer1.initialize(intervaloMedicaoVelocidade); // em microsegundos
  Timer1.attachInterrupt( contarPulsos ); // attach the service routine here

  pinMode(pinoRele, OUTPUT);
  // see if the card is present and can be initialized:
  abreCartao(true);
  chaveiaRele(true);
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
  if (currentMillis - previousMillisExibicao >= intervaloEnvio) {
    previousMillisExibicao = currentMillis;
    if (enviarDados) enviaDados();
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar != ';') {
      if (inChar != '\n') inputCodigo += inChar;
    } else {
      extraiCodigo(inputCodigo);
      inputCodigo = "";
    }
  }
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
  if (!abreCartao(false)) return;

  File dataFile = SD.open(arquivo, FILE_WRITE);
  if (dataFile) {
    String dataString = String(dados.tensao) +
                        "," + String(dados.corrente) +
                        "," + String(dados.velocidade);
    "," + String(dados.hodometro);
    "," + String(dados.temperaturaBaterias) +
    "," + String(dados.temperaturaCockpit);
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
    enviar("20", "Nao consegui abrir o arquivo, parando de gravar!");
  }
}

void incrementaPulso() {
  pulsos++;
}

void contarPulsos() {
  noInterrupts();
  dados.velocidade = ((2.0 * 3.1415 * cfg.raioRoda * pulsos) / (cfg.qtdPulsos)) * 3.6;
  dados.hodometro += (2.0 * 3.1415 * cfg.raioRoda * pulsos) / cfg.qtdPulsos;
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

void enviaDados()
{
  static float temperaturaBateriasAnterior;
  static float temperaturaCockpitAnterior;

  if (!enviarDados) return;
  enviar("01", String(dados.tensao));
  enviar("02", String(dados.corrente));
  enviar("03", String(dados.velocidade));
  enviar("04", String(dados.hodometro));
  if (abs(temperaturaBateriasAnterior - dados.temperaturaBaterias) >= STEP_TEMPERATURA) {
    enviar("05", String(dados.temperaturaBaterias));
    temperaturaBateriasAnterior = dados.temperaturaBaterias;
  }
  if (abs(temperaturaCockpitAnterior - dados.temperaturaCockpit) >= STEP_TEMPERATURA) {
    enviar("06", String(dados.temperaturaCockpit));
    temperaturaCockpitAnterior = dados.temperaturaCockpit;
  }
}

void extraiCodigo(String entrada) {
  char i = 0;
  String tmp;
  tmp.reserve(50);
  for (i = 0;; i++) {
    //aceita codigos ate 99
    if (i > 2) break;

    if (entrada[i] == ':') break;
    tmp += entrada[i];
  }
  codigo = tmp.toInt();
  switch (codigo) {
    case 1: {
        enviarDados = false;
        int intervalo = entrada.substring(i + 1, entrada.length()).toInt();
        if (intervalo > 0) {
          intervaloEnvio = intervalo;
          enviar("20", "Enviando com intervalo de: " + String(intervaloEnvio));
          enviarDados = true;
        } else {
          enviarDados = false;
          enviar("20", "Parando de enviar dados");
        }
        break;
      }

    case 2: {
        salvarDados = false;
        String arq = entrada.substring(i + 1, i + 8);
        arq.replace(" ", "");
        arq.replace("/", "-");
        if (arq.length() == 0) {
          enviar("20", "Parando de gravar dados no cartao");
          break;
        }
        if (arq.length() <= 3) {
          enviar("20", "Nome: \'" + arq + "\' muito curto");
          salvarDados = false;
          enviar("20", "Parando de gravar dados no cartao");
          break;
        }
        arquivo = arq + ".csv";
        enviar("20", "Salvando dados em " + arquivo);
        salvarDados = true;
        break;
      }
    default: {
        enviar("20", "Codigo nao reconhecido!");
        break;
      }
  }
  codigo = 0;
}
