//estrutura com os dados lidos
struct {
  float tensao;
  float corrente;
  float temperaturaBaterias;
  float temperaturaCockpit;
  float velocidade; //velocidade calculada do carro
  double hodometro; //armazena a quantidade de metros percorridos
} dados;

//estrutura com as configuracoes do controlador
struct {
  long intervaloLeitura; //intervalo de leitura dos sensores
  float raioRoda; // raio da roda em mm
  byte qtdPulsos; // pulsos por volta recebidos pelo sensor de velocidade
} cfg;


void enviar(String cod, String msg) {
  Serial.println(" {\"" + cod + "\":\"" + msg + "\"}");
}

/*
 * Funcao de regressao linear com com valores em ponto flutuante
*/
float converte(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

const int chipSelect = 10;
/*
   Abre cartao de memoria
*/
boolean abreCartao(boolean forcar) {
  static boolean cartaoOK = false;
  if (!cartaoOK || forcar) {
    cartaoOK = SD.begin(chipSelect);
    if (!cartaoOK) {
      enviar("20", "Erro ao abrir o cartao SD!");
    }
  }
  return cartaoOK;
}
