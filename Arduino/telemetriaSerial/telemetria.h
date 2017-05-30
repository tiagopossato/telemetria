const uint8_t botao = 9;
unsigned long lastDebounceTime = 0;
uint8_t debounceDelay = 50;
uint8_t lastButtonState = LOW;

const uint8_t ledVermelho =  8;
const uint8_t ledVerde =  7;

//estrutura com os dados lidos
struct {
  float tensao;
  float corrente;
  float potencia;
  float energia;
  float temperaturaBaterias;
  float temperaturaCockpit;
  float velocidade; //velocidade calculada do carro
  float hodometro; //armazena a quantidade de metros percorridos
} dados;

void enviar(String cod, String msg) {
  Serial.println(" {\"" + cod + "\":\"" + msg + "\"}");
}

/*
   Funcao de regressao linear com com valores em ponto flutuante
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
