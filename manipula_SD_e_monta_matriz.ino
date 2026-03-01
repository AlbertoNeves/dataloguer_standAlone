

// funcao para atualizar atributos data e hora para gravacao dos arquivos no cartao SD
void montaDataTempoSD(uint16_t* data, uint16_t* tempo) {
  DateTime now = RTC.now();
  horaUpdate = now.hour();
  minUpdate = now.minute();
  anoUpdate = now.year();
  mesUpdate = now.month();
  diaUpdate = now.day();
  *data = FAT_DATE(anoUpdate, mesUpdate, diaUpdate);
  *tempo = FAT_TIME(horaUpdate, minUpdate, 00);
}
//=============================================
void checaArquivo() {
  DateTime now = RTC.now();
  uint16_t qtdPosicoes = now.hour() * 20 + now.minute() * 3;

  for (int j = 0; j < qtdPosicoes; j++) {
    if (matrizExibicao[j] == 0) {
      for (byte tipo_t_u_p = 0; tipo_t_u_p <= 2; tipo_t_u_p++) {
        grava(nomeArquivoDia, tipo_t_u_p);  // funcao grava, informando o nome do arquivo a ser gravado, tipo de arquivo e extencao.
      }                                     //end for byte tipo_t_u_p
    }
  }
}
//=============================================

void grava(String nomeArquivo, byte tipo_t_u_p) {
  String dataString = "";  // cria uma string para conter dado Temperatura ou Umidade ou Pressao e gravar no SD

  switch (tipo_t_u_p) {
    case 0:
      nomeArquivo += ".tem";  // acrescenta .tem "Temperatura A cada Minuto" a string recebida anoMesDia
      dataString = temperaturaAtual;
      break;
    case 1:
      nomeArquivo += ".umi";  // acrescenta .uam "Umidade A cada Minuto"a string recebida anoMesDia
      dataString = umidadeAtual;
      break;
    case 2:
      nomeArquivo += ".pre";  // acrescenta .pam "Pressao A cada Minuto"a string recebida anoMesDia
      dataString = pressaoAtual;
      break;
    case 3:
      nomeArquivo += ".bat";  // acrescenta .bat
      dataString = analogRead(pinoBateria) * (5.0 / 1023.0);

      if (!SD.exists(nomeArquivo)) {
        SdFile::dateTimeCallback(montaDataTempoSD);  // chama funcao que atualiza data e hora nos atributos do arquivos a ser gravado
      }
      break;
  }  //end switch tipo_t_u_P


  File dataFile = SD.open(nomeArquivo, FILE_WRITE);  // abre, cria arquivo, salva
  if (dataFile) {                                    // se o arquivo for valido, salve os dados:
    dataFile.println(dataString);                    // salva no arquivo
    dataFile.close();
    sdERRO = 0;
  } else {
    //se o arquivo não estiver aberto, digite um erro:
    msgErroSD(nomeArquivo);
    sdERRO = 1;
  }
  if (!SD.exists(nomeArquivo)) {
    SdFile::dateTimeCallbackCancel();  // para que nao haja alteracao dos atributos do arquivo, assim sera gravado a criacao e nao a ultima modificacao
  }
}  // end GRAVA
//=======================================================

// para usar no exxel
void gravarExcel(String nomeArquivo, String hora, String minuto) {

  String dadosTUP = "";  // cria uma string para conter temp, umi, press a cada 3 minutos

  if (cabecalho == 0) {
    if (SD.exists(nomeArquivo)) {
      cabecalho = 1;
    } else {
      dadosTUP = "Hora:Minuto | Temperatura | Umidade |  Pressao";
      cabecalho = 1;
    }
  }  //end  if (cabecalho == 0)

  if (cabecalho == 2) {
    dadosTUP += "  ";
    dadosTUP += String(hora);
    dadosTUP += String(":");
    dadosTUP += String(minuto);
    dadosTUP += "     |     ";
    dadosTUP += String(temperaturaAtual);
    dadosTUP += "   |  ";
    dadosTUP += String(umidadeAtual);
    dadosTUP += "  |  ";
    dadosTUP += String(pressaoAtual);
  }  // end if  cabecalho = 2

  cabecalho = 2;  // nessa posicao, primeiro grava o cabecalho e na proxima volta grava os dados

  nomeArquivo += ".csv";

  File dataFile = SD.open(nomeArquivo, FILE_WRITE);  // abre, cria arquivo, grava dados
  if (dataFile) {                                    //  se o arquivo for valido, salve os dados:
    dataFile.println(dadosTUP);                      // salva no arquivo
    dataFile.close();
    //Serial.print("gravado "); Serial.println(nomeArquivo);
    sdERRO = 0;
  } else {
    //se o arquivo não estiver aberto, digite um erro:
    msgErroSD(nomeArquivo);
    sdERRO = 1;
  }
}
//------------------------------------------------------------------

void montaMatriz(String arquivoAler) {

  File dataFile = SD.open(arquivoAler);  //
  uint16_t indice = 0;                   // para numerar o indice da matriz
  String recebeDado = "";                // string para concatenar os valores lido do SD , que veem em ascII

  if (dataFile) {                                          //
    while (dataFile.available()) {                         // enquanto houver dados no arquivo do sd
      char dadoSD = dataFile.read();                       // variavel que vai receber os dado vindo do SD
      if (isDigit(dadoSD)) {                               // se forem numeros ,
        recebeDado += (char)dadoSD;                        // adicione os à string:
      }                                                    // end if isDigit
      if (dadoSD == '\n') {                                // quando receber comando "nova linha, converta a string em inteiro e salve na matriz
        matrizExibicao[indice] = recebeDado.toInt() / 10;  // antes de adicionar esse valor a matriz,  divida por 10
        indice++;                                          // para numerar o indice da matriz
        recebeDado = "";                                   // limpa a String para proxima conversao
      }                                                    // end if (dadoSD == '\n') '/n' equivale a new line
    }                                                      // end while (dataFile.available()
    dataFile.close();                                      // fecha arquivo do SD
    sdERRO = 0;
   
  }                               // end  if (dataFile)
  else {                          // se o arquivo não abrir:
    msgErroSD(arquivoAler);       // va para funcao mensagem de eero com o nome do arquivo a se exibido
    sdERRO = 1;
  }  // end else se arquivo nao abrir

  //uint16_t numElementos = sizeof(matrizExibicao) / sizeof(matrizExibicao[0]);


  //Serial.println("==============================================");
  //Serial.print("Numero elementos existentes na matriz =");
  //Serial.println(numElementos);
  // Serial.print("Quantidade de posicoes em relacao ao tempo =");
  // Serial.println(qtdPosicoes);
  // Serial.println("==============================================");

  //for (int j = 0; j < numElementos; j++) {
  //   Serial.print("posicao matriz = ");
  //   Serial.print(j);
  //   Serial.print("  dado nessa posicao = ");
  //  Serial.println(matrizExibicao[j]);  //= 0;
  // }

}  // end montaMatriz

//------------------------------------------------------------------
/*
  // sizeof(matrizExibicao) / sizeof(matrizExibicao[0]); <<para funcionar corretamente com uma matriz de qualquer tipo ou tamanho)

  if (determinaYmaxYmin == 0) {
    for (byte i = 0; i < sizeof(matrizExibicao) / sizeof(matrizExibicao[0]); i++) {
      if (escala_Y_max < matrizExibicao[i]) {                         // compara cada valor da matriz com escala_Y_max
        escala_Y_max = matrizExibicao[i];                             // e determina o maior valor
      }                                                                  // end compara cada valor da matriz com escala_Y_max
      if (matrizExibicao[i] < escala_Y_min) {                         // compara cada valor da matriz com escala_Y_min
        escala_Y_min = matrizExibicao[i];                             // e determina o menor valor
      }                                                         // end compara cada valor da matriz com escala_Y_min
    }                                                           // end laco for varrendo matriz procurando maximos e minimos
  determinaYmaxYmin=1;
  }// end if determinaYmaxYmin
*/
//==============================================
/*
  Serial.println("===================================");
  Serial.print ("Temperatura Maxima ");  Serial.println(escala_Y_max);
  Serial.println("===================================");
  Serial.print ("Temperatura Minima ");  Serial.println(escala_Y_min);
  Serial.println("===================================");

  //==============================================
*/
