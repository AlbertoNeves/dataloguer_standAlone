/*
*************************** Conexões ***************************************************
   Modulo cartao SD            
  SD          AVR 2560         
  MOSI ------  51               
  MISO ------  50                  
  CLK  ------  52              
  CS   ---*/
const int CS_SD = 53;        /*  
  5V  ------   5V                                           
  GND  ------  GND    


  modulo rtc /BMP280              
 RTC         AVR 2560       
 SDA -------- 20             
 SCL -------- 21             
 VCC -------- 3.3V           
 GND -------- GND           
                            
VCC BME280 chaveado por transistor PNP para nao aquecer             
*/
const int onOffBME = 12;     /* chaveamento do sensor BME280, ligar antes de fazer leitura, 
evita aquecimento do shield,  LOW liga/HIGH desliga     

    Teclado  
Comum ------- GND 
Home ----- D2  */
const int pinhome = 2;       /* 
Baixo ------- D4  */
const int pinDown = 4;       /* 
Cima -------- D6  */
const int pinUp = 6;         /* 
Esquerda ---- D8  */
const int pinLeftPrev = 8;   /* 
Direita ----- D10 */
const int pinRightNext = 10; /* 


     display 128X64-controlador ST7920-SPI      
  1-GND --------  GND     
  2-VCC --------  5V    
  3-----contrast-- */
const int pinContraste = 3;  /* pwm contraste display (diodo)
  4-RS    -------- */
const int U8G2_CS = 5;       /*,  "CS"    
  5-R/W   -------- */
const int U8G2_DATA = 7;     /*,  "DATA"
  6-E     -------- */
const int U8G2_CLOCK = 9;    /*  "CLOCK"
  7...14---NC-----
  15--Paralel/SERIAL - GND   
  16-------NC----- */
const int pinBackLight = 11; /*  liga/desliga backlight display BJT NPN
  17-RESET-------- */
const int U8G2_RESET = 13;   /*  "RESET"
  18-------NC----- 
  19-BACK LIGTH ANODE
  20-BACK LIGTH CATODE

 */
//############################################################

#include <Arduino.h>
#include <Chave.h>  //biblioteca para usara booes pressionado e pressionado por X tempo
#include <SPI.h>
#include <SD.h>  // versao 1.3.0
//nao precisa incluir, mas tem que estar instalada <SdFat.h> versao 2.3.0
File dataFile;
Sd2Card card;
SdVolume volume;
SdFile root;

#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;  // seta nome RTC (capsLock UP) para referencia e uso

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>            // usar bme.begin(0x76) para indicar o endereco do BME, que tambem pode ser bme.begin(0x76))
#define SEALEVELPRESSURE_HPA (1013.25)  // nivel do mar (1013.25)
Adafruit_BME280 bme;                    // seta nome bme  para referencia e uso e I2C

#include <U8g2lib.h>
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R2, U8G2_CLOCK, U8G2_DATA, U8G2_CS, U8G2_RESET);
/*
#include <OneWire.h>
#include <DallasTemperature.h>
const int busOneWire = 15;            // define pino utilizado para cominicacao oneWire
OneWire oneWire(busOneWire);          // Prepara uma instância oneWire para comunicar com qualquer outro dispositivo oneWire
DallasTemperature sensors(&oneWire);  // Passa uma referência oneWire para a biblioteca DallasTemperature
*/

#include <EEPROMex.h>
// if (brilho != EEPROM.read(10)) EEPROM.update(10, brilho);
// if (contraste != EEPROM.read(20)) EEPROM.update(20, contraste);
// if (offSetTemp != EEPROM.read(30)) EEPROM.update(30, offSetTemp);

//------------------------------------------------------------------------------------
// valores fixos

uint8_t maxT = 0;
uint8_t minT = 100;
uint8_t maxU = 0;
uint8_t minU = 100;

const uint8_t originX = 11;    // ponto ZERO do eixo X
const uint8_t originY = 53;    // ponto ZERO do eixo Y
const uint8_t tamanhoX = 128;  // quantidade de pixels X=tamanho do display
const uint8_t tamanhoY = 64;   // quantidade de pixels y=tamanho do display

uint8_t selecao = 0;        // para fazer switch/case das telas
uint32_t tempo = millis();  // para fazer switch/case das telas

bool onOff = 0;              // variavel on/off a cada 500 ms, faz parte de tempoPisca
uint8_t passou3minutos = 4;  // para contar de 3 em 3 minutos  < comeca com 4 para ja gravar arquivo com nome do dia
uint8_t passou10minutos = 0;
uint32_t passou3seg = 0;       // contar tempo entrar menu ajuste
uint16_t matrizExibicao[480];  // monta matriz com 480 campos -uso diario // retira do SD(em string) a tripa de valores e guardar em array int
//uint16_t matrizUmidade[480];
//uint16_t matrizPressao[480];

float temperaturaAtual = 0.0;       // guarda leitura temperatura
float offSetTemp = 0.0;             // off set de temperatura
float umidadeAtual = 0.0;           // guarda leitura umidade
float pressaoAtual = 0.0;           // guarda leitura pressao
uint16_t tempAnterior = 0.0;        // para guardar a temperatura diferente
uint16_t tempSai = 0;               // para fazer media de temperatura e utilizar no cartesiano
uint16_t escala_Y_max = 0;          // valor maximo da array  matrizExibicao, esse define o valor maximo do eixo Y
uint16_t escala_Y_min = 200;        // valor minimo da array  matrizExibicao, esse define o valor minimo do eixo Y
uint16_t escalaY_Mx_Mn = 0;         // para fazer a media entre escala_Y_max e escala_Y_min
uint8_t multiplicadorEscalaY = 1;   // para modificar a escala do eixo Y
uint8_t controlaIndiceMatriz = 0;  // para exibicao manual ou automatica da matriz Temperatura
//uint8_t controlaIndiceMatrizU = 30;  // para exibicao manual ou automatica da matriz Umidade
//uint8_t controlaIndiceMatrizP = 30;  // para exibicao manual ou automatica da matriz Pressao

bool sdERRO = 0;             // se for 1, liga icone no LCD
bool rtcERRO = 0;            // se for 1, liga icone no LCD
uint8_t pinoBateria = A0;    // pino leitura tensao da bateria
uint16_t tensaoBateria = 0;  // valor da tensao da bateria
uint8_t posiCursor = 0;      // posiCursor para movimentar cursor de escolha
uint8_t minutoAnterior = 61;
uint8_t horaAnterior = 25;
uint8_t diaAnterior = 0;
uint8_t mesAnterior = 13;
uint8_t anoAnterior = 0;

uint8_t horaUpdate;
uint8_t minUpdate;
uint16_t anoUpdate;
uint8_t mesUpdate;
uint8_t diaUpdate;
uint8_t AdjClockFuncao = 0;
uint8_t conta_tempo;  // a cada incremento de(500ms), altera onOff que habilita/desabilita  icones no LCD
//                    // e a cada 20 ciclos (10 segundos) alternar a variavel selecao que controla a tela de exibicao.

uint8_t pos_x = 50;  //  em qual pixel comeca a exibicao data hora
uint8_t valorX = 0;  // calculado na funcao traca linha, usado na indicacao das horas no eixo X

String command;              // para receber os dados da serial e gerar comando
String nomeArquivoDia = "";  // variavel para gerar nome para arquivo diario

uint8_t cabecalho = 0;  // utilizado a cada novo arquivo CSV , para colocar nome nas colunas de dados

bool tela = 0;           // libera exibicao tela um avez por ciclo ou alteracao de botoes
uint8_t tempON = 0;      // tempo que tela fica acesa
uint8_t brilho = 50;     // controla nivel  backlight display
uint8_t contraste = 70;  // controla nivel DC contraste display
bool posBriCon = 0;      // 0 para akjustar brilho, 1 para ajustar contraste

Chave home(pinhome);
Chave RightNext(pinRightNext);
Chave LeftPrev(pinLeftPrev);
Chave Up(pinUp);
Chave Down(pinDown);

//------------------------------------------------------------------------------------
//prototipo das funcoes
void traca_Linha();
void montaMatriz();
void traca_cartesiano();
void DisplayDateTime();
void DisplaySetHour();
void DisplaySetMinute();
void DisplaySetYear();
void DisplaySetMonth();
void DisplaySetDay();
void salvaRelogio();
void help();
void lerSerial();
void leSensores();
void printdiretorio();
void montaDataHora();
void montaDataTempoSD();
void msgErroSD();
void gravarExcel();
void grava();
void Error();
void checaBotao();

//------------------------------------------------------------------------------------

/*void setup() {

  brilho = EEPROM.read(10);
  contraste = EEPROM.read(20);
  offSetTemp = EEPROM.readFloat(30);

  Serial.begin(115200);
  //---------------
  //sensors.begin();  // Inicia a biblioteca para comunicar oneWire
  //---------------

  pinMode(onOffBME, OUTPUT);
  digitalWrite(onOffBME, HIGH);

  pinMode(pinBackLight, OUTPUT);
  analogWrite(pinBackLight, brilho);

  pinMode(pinContraste, OUTPUT);
  analogWrite(pinContraste, contraste);
  //---------------
  analogReference(DEFAULT);
  //---------------
  Wire.begin();  // Inicia a biblioteca para comunicar I2C
  //---------------
  delay(100);  // aguarda protocolos de comunicacao estabilizar
  if (!RTC.begin()) {
    RTC.begin();
    RTC.adjust(DateTime(2025, 10, 22, 17, 44, 0));
    RTC.writeSqwPinMode(DS1307_SquareWave1HZ);
    delay(600);
    Error(0);
  } else {
    RTC.begin();
    RTC.writeSqwPinMode(DS1307_SquareWave1HZ);
    Serial.println(" R.T.C inicializado");
    Serial.println("=============================");
  }
  //if (!RTC.isrunning()) {
  //  RTC.adjust(DateTime(2025, 10, 22, 17, 44, 0));
  // }
  //---------------
  digitalWrite(onOffBME, LOW);
  delay(50);
  if (!bme.begin(0x76)) {
    Error(1);
  } else {
    Serial.println(" SENSOR BME280 inicializado");
    Serial.println("=============================");
    digitalWrite(onOffBME, HIGH);
  }
  //---------------
  SD.begin(CS_SD);
  if (!card.init(SPI_HALF_SPEED, CS_SD)) {
    Error(2);
    sdERRO = 1;  // flag indicativa de erro no cartao SD
  } else {
    Serial.println(" cartao inicializado.");
    Serial.println("=============================");
    sdERRO = 0;  // libera flag indicativa de erro do cartao SD
  }
  //---------------
  //determina pinos para botoes, usado na biblioteca u8g2
  u8g2.begin();  //

  Serial.println(" Dataloguer temperatura/umidade/pressao barometrica ");
  Serial.println(" by alberto.alves.neves@gmail.com , junho 2025, BRAZIL");
  Serial.println(" ver 2.2 ");
  Serial.println(" D:_Meus documentos_Google Drive_ARDUINO-pasta-meus-docs-todos-os-pcs_LCD-grafico-texto_Graficos_LCD 128X64_dataloguer_LCD_com_cartao_SD-mega-v22");
  Serial.println(" serial 115200,8,n,1");
  tensaoBateria = analogRead(pinoBateria);
  Serial.print(" TensaoBateria = ");
  Serial.print(tensaoBateria * (5.0 / 1023.0));
  Serial.println("V");
  //---------------
  help();                                      // apresenta mensagem no monitor serial dos comandos via serial
                                               //---------------
  SdFile::dateTimeCallback(montaDataTempoSD);  // chama funcao que atualiza data e hora nos atributos do arquivos a ser gravado

}  //end setup*/

void setup() {
  // 1. Recupera configurações da EEPROM
  brilho = EEPROM.read(10);
  contraste = EEPROM.read(20);
  offSetTemp = EEPROM.readFloat(30);

  Serial.begin(115200);
  
  // 2. Configura Pinos e Periféricos
  pinMode(onOffBME, OUTPUT);
  digitalWrite(onOffBME, LOW); // Liga o sensor (assumindo lógica low = on ou reset)

  pinMode(pinBackLight, OUTPUT);
  analogWrite(pinBackLight, brilho);

  pinMode(pinContraste, OUTPUT);
  analogWrite(pinContraste, contraste);
  
  Wire.begin(); 
  delay(200); // Tempo maior para estabilização de todos os escravos I2C

  // 3. Inicialização do RTC (DS1307)
  if (!RTC.begin()) {
    Serial.println(F("Erro: RTC nao encontrado!"));
    Error(0); 
  } else {
    if (!RTC.isrunning()) {
      Serial.println(F("RTC parado, ajustando data..."));
      RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajusta para hora da compilação
    }
    RTC.writeSqwPinMode(DS1307_SquareWave1HZ);
    Serial.println(F("R.T.C OK"));
  }

  // 4. Inicialização do BME280
  if (!bme.begin(0x76)) {
    Serial.println(F("Erro: BME280 nao encontrado!"));
    Error(1);
  } else {
    Serial.println(F("Sensor BME280 OK"));
  }
 digitalWrite(onOffBME, HIGH); // Desiga o sensor (assumindo lógica high = off)

  // 5. Inicialização do Cartão SD
  // Nota: card.init e SD.begin juntos podem conflitar. O padrão é usar apenas SD.begin.
  if (!SD.begin(CS_SD)) {
    Serial.println(F("Erro: Falha no Cartao SD!"));
    sdERRO = 1;
    Error(2);
  } else {
    Serial.println(F("Cartao SD OK"));
    sdERRO = 0;
  }

  // 6. Interface e Infos
  u8g2.begin();
  
  // Uso da Macro F() para economizar RAM em strings longas
  Serial.println(F("Datalogger v2.2 - Inicializado"));
  
  tensaoBateria = analogRead(pinoBateria);
  float voltagem = tensaoBateria * (5.0 / 1023.0);
  Serial.print(F("Bateria: "));
  Serial.print(voltagem);
  Serial.println("V");

  help();                                      
  SdFile::dateTimeCallback(montaDataTempoSD); 
}

//------------------------------------------------------------------------------------
void loop(void) {
  unsigned long currentMillis = millis();

  // --- Controle de Tempo (500ms) ---
  if (currentMillis - tempo >= 500) {
    tempo = currentMillis;
    conta_tempo++;
    tempON++;
    onOff = !onOff; // Usado para o ícone piscante

    // Apaga backlight após 40 segundos (80 * 500ms)
    if (tempON >= 80) {
      brilho = 0;
      analogWrite(pinBackLight, brilho);
      tempON = 0;
    }

    tensaoBateria = analogRead(pinoBateria);

    // Controle de menus/telas
    if (conta_tempo >= 20) {
      if (selecao <= 5) selecao++;
      else if (selecao == 4 || selecao >= 10) selecao = 0;
      
      tela = 0;
      conta_tempo = 0;
      controlaIndiceMatriz = 0;
    }
  }

  DateTime now = RTC.now();
  char bufferNome[13]; // Buffer para armazenar "YYMMDD" (8.3 format)

  // --- Mudança de Dia: Gera novo nome de arquivo ---
  if (now.day() != diaAnterior) {
    diaAnterior = now.day();
    memset(matrizExibicao, 0, sizeof(matrizExibicao));
    cabecalho = 0;

    // Formata o nome base: YYMMDD (Ex: 250615)
    snprintf(bufferNome, sizeof(bufferNome), "%02d%02d%02d", 
             now.year() % 100, now.month(), now.day());
    nomeArquivoDia = String(bufferNome); // Converte uma única vez
    
    SdFile::dateTimeCallback(montaDataTempoSD);
  }

  // --- Verificação de Minuto ---
  if (now.minute() != minutoAnterior) {
    minutoAnterior = now.minute();
    passou3minutos++;
    passou10minutos++;
    leSensores();

    // --- Gravação a cada 3 Minutos ---
    if (passou3minutos >= 3 || (now.hour() == 0 && now.minute() == 0)) {
      passou3minutos = 0;

      char hStr[3], mStr[3];
      snprintf(hStr, 3, "%02d", now.hour());
      snprintf(mStr, 3, "%02d", now.minute());

      gravarExcel(nomeArquivoDia, String(hStr), String(mStr));

      for (byte i = 0; i <= 2; i++) {
        grava(nomeArquivoDia, i); 
      }

      montaMatriz(nomeArquivoDia + ".tem");
      SdFile::dateTimeCallbackCancel();
    }

    // --- Gravação da Bateria (10 Minutos) ---
    if (passou10minutos >= 10) {
      passou10minutos = 0;
      grava(nomeArquivoDia, 3); // .bat
    }
  }

  lerSerial();
  checaBotao();
  
  
}

//------------------------------------------------------------------------------------
/*void loop(void) {

  unsigned long currentMillis = millis();

  if (currentMillis - tempo >= 500) {
    conta_tempo++;
    tempON++;
    onOff = !onOff;
    if (tempON >= 80) {
      brilho = 0;
      analogWrite(pinBackLight, brilho);
      tempON = 0;
    }

    tensaoBateria = analogRead(pinoBateria);

    if (conta_tempo >= 20) {
      if (selecao <= 5) {
        selecao++;
      } else if ((selecao == 4) || (selecao >= 10)) {
        selecao = 0;
      }

      tela = 0;
      conta_tempo = 0;
      controlaIndiceMatriz = 0;  // se nenhum botao for precionado em ate 10 segundos,volta para automatico
    }                             // end if conta_tempo >= 20
    tempo = currentMillis;
  }  // end if (millis() >= tempo + 500)


  //-----------------------------------------------------------
  String nomeArquivoEXC = "";
  String hora = "";
  String minuto = "";
  //---------------------------------------------------------
  DateTime now = RTC.now();
  //---------------------------------------------------------
  if (now.day() != diaAnterior) {
    diaAnterior = now.day();

    memset(matrizExibicao, 0, sizeof(matrizExibicao));  //limpa matriz matrizExibicao

    cabecalho = 0;                               // utilizado a cada novo arquivo CSV , para colocar nome nas colunas de dados
    SdFile::dateTimeCallback(montaDataTempoSD);  // chama funcao que atualiza data e hora nos atributos do arquivos a ser gravado

    nomeArquivoDia = "";                          // variavel para gerar nome para arquivo diario
    nomeArquivoDia += String(now.year() - 2000);  // montar nome do arquivo a ser gravado

    if (now.month() <= 9) {
      nomeArquivoDia += "0";
    }
    nomeArquivoDia += String(now.month(), DEC);

    if (now.day() <= 9) {
      nomeArquivoDia += "0";
    }
    nomeArquivoDia += String(now.day(), DEC);


  }  // end if  (now.day() != diaAnterior)
  //----------------------------------------------------------------------------
  if (now.minute() != minutoAnterior) {
    passou3minutos++;
    passou10minutos++;
    minutoAnterior = now.minute();
    leSensores();
  }  // end if verificando se mudou o minuto
  //----------------------------------------------------------------------------
  if ((passou3minutos >= 3) || (now.hour() == 0) && (now.minute() == 0)) {
    passou3minutos = 0;
    if (now.hour() <= 9) {
      hora += "0";
    }
    hora += String(now.hour(), DEC);

    if (now.minute() <= 9) {
      minuto += "0";
    }
    minuto += String(now.minute(), DEC);

    gravarExcel(nomeArquivoDia, hora, minuto);

    for (byte tipo_t_u_p = 0; tipo_t_u_p <= 2; tipo_t_u_p++) {
      grava(nomeArquivoDia, tipo_t_u_p);  // funcao grava, informando o nome do arquivo a ser gravado, tipo de arquivo e extencao.
    }                                     //end for byte tipo_t_u_p

    montaMatriz(nomeArquivoDia + ".tem");  // vai ler o SD e passar para matriz de Temperatura , que sera exibida nao LCD
    // montaMatriz(nomeArquivoDia + ".umi");               // vai ler o SD e passar para matriz de Umidade , que sera exibida nao LCD
    // montaMatriz(nomeArquivoDia + ".pre");               // vai ler o SD e passar para matriz de Pressao , que sera exibida nao LCD

    // checaArquivo(); // <=================

    SdFile::dateTimeCallbackCancel();  // para que nao haja alteracao dos atributos do arquivo, assim sera gravado os atributos na criacao e nao na ultima modificacao

  }  // end if verificando se passaram 3 minutos
  //---------------------------------------------------------
  if (passou10minutos >= 10) {
    passou10minutos = 0;

    grava(nomeArquivoDia, 3);


  }  // end if verificando se passaram 10 minutos

  //---------------------------------------------------------

  lerSerial();
  checaBotao();

}  // end void loop*/
