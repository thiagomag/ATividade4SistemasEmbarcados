// -----------------------------------------------------------
// Estação Meteorológica com Arduino
// Desenvolvido para a disciplina Sistemas Embarcados
// Professor: Felipe de Oliveira Baldner
// 
// Funcionalidades:
// - Medir temperatura e umidade usando o sensor DHT11
// - Medir pressão atmosférica usando o sensor BMP180
// - Detectar chuva usando um botão (simulando um sensor de chuva)
// - Exibir dados no display LCD 16x2 ou OLED
// - Indicar chuva no display de 7 segmentos (exibe "C" se estiver chovendo)
// -----------------------------------------------------------

#include <DHT.h> // Biblioteca para o sensor DHT11
#include <Wire.h> // Biblioteca I2C padrão
#include <Adafruit_Sensor.h> // Biblioteca base para sensores Adafruit
#include <Adafruit_BMP085_U.h> // Biblioteca para o sensor BMP180
#include <LiquidCrystal_I2C.h> // Biblioteca para o display LCD com interface I2C

// Configuração do sensor DHT11
#define DHTPIN 2       // Pino de dados do DHT11
#define DHTTYPE DHT11  // Tipo do sensor DHT (neste caso, DHT11)
DHT dht(DHTPIN, DHTTYPE);

// Configuração do sensor BMP180
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); // Objeto do sensor BMP180

// Configuração do botão de chuva
#define CHUVA_PIN 3 // Pino onde o botão de chuva está conectado

// Configuração do display LCD 16x2 (endereço I2C padrão 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuração do display de 7 segmentos (pinos para os segmentos)
#define SEG_A 8
#define SEG_B 9
#define SEG_C 10
#define SEG_D 11
#define SEG_E 12
#define SEG_F 13
#define SEG_G 7

// -----------------------------------------------------------
// Configuração inicial do sistema
// -----------------------------------------------------------
void setup() {
  // Inicialização da comunicação serial para debug
  Serial.begin(9600);

  // Inicialização do sensor DHT11
  dht.begin();

  // Inicialização do sensor BMP180
  if (!bmp.begin()) {
    Serial.println("Erro ao inicializar o sensor BMP180!");
    while (1); // Travar o sistema em caso de erro crítico
  }

  // Inicialização do display LCD
  lcd.init();
  lcd.backlight(); // Ativar luz de fundo do display

  // Configuração do botão de chuva
  pinMode(CHUVA_PIN, INPUT_PULLUP); // Configurado com resistor interno de pull-up

  // Configuração dos pinos do display de 7 segmentos
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  // Garantir que o display de 7 segmentos inicie desligado
  desligarDisplay7Segmentos();
}

// -----------------------------------------------------------
// Loop principal do programa
// -----------------------------------------------------------
void loop() {
  // Leitura da temperatura e umidade do DHT11
  float temperatura = dht.readTemperature(); // Temperatura em graus Celsius
  float umidade = dht.readHumidity();        // Umidade relativa em %

  // Leitura da pressão atmosférica do BMP180
  sensors_event_t evento;
  bmp.getEvent(&evento);
  float pressao = evento.pressure; // Pressão em hPa

  // Verificação do estado do botão de chuva
  bool estaChovendo = !digitalRead(CHUVA_PIN); // Invertido devido ao pull-up interno

  // Atualização do display LCD com as leituras
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temperatura, 1); lcd.print("C ");
  lcd.print("H:"); lcd.print(umidade, 1); lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("P:"); lcd.print(pressao, 1); lcd.print("hPa");

  // Atualização do display de 7 segmentos
  if (estaChovendo) {
    mostrarLetraC(); // Exibir a letra "C" no display de 7 segmentos
  } else {
    desligarDisplay7Segmentos(); // Desligar o display de 7 segmentos
  }

  delay(1000); // Atualizar as informações a cada 1 segundo
}

// -----------------------------------------------------------
// Função para exibir a letra "C" no display de 7 segmentos
// -----------------------------------------------------------
void mostrarLetraC() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
}

// -----------------------------------------------------------
// Função para desligar o display de 7 segmentos
// -----------------------------------------------------------
void desligarDisplay7Segmentos() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}

// -----------------------------------------------------------
// Notas importantes:
// - Conectar os sensores corretamente conforme o código.
// - Utilizar resistores de pull-up ou pull-down, quando necessário.
// - Certificar-se de usar as bibliotecas corretas disponíveis no Gerenciador de Bibliotecas do Arduino IDE.
// -----------------------------------------------------------
