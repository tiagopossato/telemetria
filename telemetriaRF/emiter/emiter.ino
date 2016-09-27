#include <VirtualWire.h>
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

long intervaloEnvio = 150; //150 ms

void setup() {
  cfg.raioRoda = 0.255;
  cfg.qtdPulsos = 4;
  cfg.intervaloLeitura = 100;

  // Initialise the IO and ISR
  vw_set_tx_pin(4); // Define o pino 4 do Arduino como entrada
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec

  Serial.begin(9600);
  pinMode(pinoVelocidade, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinoVelocidade), incrementaPulso, FALLING); //FALLING for when the pin goes from high to low.

  Timer1.initialize(intervaloMedicaoVelocidade); // em microsegundos
  Timer1.attachInterrupt( contarPulsos ); // attach the service routine here

  pinMode(pinoRele, OUTPUT);
  // see if the card is present and can be initialized:
  chaveiaRele(true);
}

void loop() {
  static unsigned long previousMillisLeitura = 0;
  static unsigned long previousMillisExibicao = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisLeitura >= cfg.intervaloLeitura) {
    previousMillisLeitura = currentMillis;
    lerSensores();
  }
  if (currentMillis - previousMillisExibicao >= intervaloEnvio) {
    previousMillisExibicao = currentMillis;
    enviaDados();
  }
}

void lerSensores() {
  long lido = 0;
  //tensao
  for (int i = 0; i < AMOSTRAS; i++) lido += analogRead(pinoTensao);
  //converte e aplica curva de calibração no valor lido
  dados.tensao = 100 * 0.96992571774 * converte((float) lido / AMOSTRAS, 0.0, 1023.0, 0.0, 60.0) + 0.3008745324;
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
void incrementaPulso() {
  pulsos++;
  dados.hodometro += (2.0 * 3.1415 * cfg.raioRoda) / cfg.qtdPulsos;
}

void contarPulsos() {
  noInterrupts();
  dados.velocidade = ((2.0 * 3.1415 * cfg.raioRoda * pulsos) / (cfg.qtdPulsos)) * 3.6;
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

  //enviar("01", String(dados.tensao));
  char _dadoys[40];

  sprintf(_dadoys, "{\"01\":\"%d\"}\0", dados.tensao);
  const char *_dados = "hello";
  Serial.println(_dados);
  vw_send((uint8_t *)_dados, strlen(_dados));
  vw_wait_tx(); // Wait until the whole message is gone
  //
  //  enviar("02", String(dados.corrente));
  //  enviar("03", String(dados.velocidade));
  //  enviar("04", String(dados.hodometro));
  //  if (abs(temperaturaBateriasAnterior - dados.temperaturaBaterias) >= STEP_TEMPERATURA) {
  //    enviar("05", String(dados.temperaturaBaterias));
  //    temperaturaBateriasAnterior = dados.temperaturaBaterias;
  //  }
  //  if (abs(temperaturaCockpitAnterior - dados.temperaturaCockpit) >= STEP_TEMPERATURA) {
  //    enviar("06", String(dados.temperaturaCockpit));
  //    temperaturaCockpitAnterior = dados.temperaturaCockpit;
  //  }
}
