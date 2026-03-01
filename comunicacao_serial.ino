//https://forum.arduino.cc/index.php?topic=288234.0
//https://www.norwegiancreations.com/2017/12/arduino-tutorial-serial-inputs/


void lerSerial() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');

    if ((command.equals("dir")) || (command.equals("DIR"))) {
      printdiretorio();
    } else if ((command.equals("ler")) || (command.equals("LER"))) {
      ler();
    } else if ((command.equals("apagar")) || (command.equals("APAGAR"))) {
      apagar();
    } else if ((command.equals("data")) || (command.equals("DATA"))) {
      montaDataHora();
    } else if ((command.equals("help")) || (command.equals("HELP"))) {
      help();
    } else if ((command.equals("matrizT")) || (command.equals("MATRIZT"))) {
      matrizT();
    } /*
    else if ((command.equals("matrizU")) || (command.equals("MATRIZU"))) {
        matrizU();
      }
    else if ((command.equals("matrizP")) || (command.equals("MATRIZP"))) {
        matrizP();
      }*/
    else {
      Serial.println("Comando Invalido, digite help");
    }
  }  //end Serial avaliable
}  // end void lerSeria
//============================================================
void help() {
  Serial.println("==================================================");
  Serial.println(" Digite    data      para ver data e hora do sistema");
  Serial.println(" Digite    dir       para ver os nomes de arquivos");
  Serial.println(" Digite    ler       para visualizar um arquivo pela serial");
  Serial.println(" aguarde sistema pedir para digitar o nome do arquivo");
  Serial.println(" Digite o nome do arquivo com a extencao em MAISCULA ");
  Serial.println(" Digite    apagar    para deletar um arquivo no cartão SD pela serial");
  Serial.println(" aguarde sistema pedir para digitar o nome do arquivo");
  Serial.println(" Digite o nome do arquivo com a extencao em MAIUSCULA");
  Serial.println(" ");
  Serial.println(" O nome do arquivo é composto por 6 digitos para arquivo diario");
  Serial.println("sendo 02 digitos do ano, 02 digitos do mes, 02 digitos do dia");
  Serial.println("Assim 200316 é igual a ");
  Serial.println("ano  20||||||");
  Serial.println("mes  ||03|||| ");
  Serial.println("dia  ||||16||");
  Serial.println("==================================================");
  /*
  Serial.println(" Digite   matrizT    para ver os dados da matriz Temperatura");
  Serial.println(" Digite   matrizU    para ver os dados da matriz Umidade");
  Serial.println(" Digite   matrizP    para ver os dados da matriz Pressao");
  Serial.println("==================================================");
*/
}  // end void help
//============================================================

void printdiretorio() {
  Serial.println("==================================================");
  Serial.println(" Arquivos presentes no SD ");
  card.init(SPI_HALF_SPEED, CS_SD);
  volume.init(card);
  root.openRoot(volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);  // list all files in the card with date and size
  dataFile.close();
  Serial.println("==================================================");
}  // end printdiretorio
//============================================================

void montaDataHora() {
  DateTime now = RTC.now();
  Serial.println("==================================================");
  Serial.println("Data e hora do RTC");
  if (now.day() <= 9) {
    Serial.print("0");
  }
  Serial.print(now.day());
  Serial.print("/");
  if (now.month() <= 9) {
    Serial.print("0");
  }
  Serial.print(now.month());
  Serial.print("/");
  Serial.println(now.year());

  if (now.hour() <= 9) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() <= 9) {
    Serial.print("0");
  }
  Serial.print(now.minute());
  Serial.print(":");
  if (now.second() <= 9) {
    Serial.print("0");
  }
  Serial.println(now.second());
  Serial.println("==================================================");
}  //end void data hora
//============================================================

void ler() {

  String arquivo;

  Serial.println("Digite o nome do arquivo com extencao ");
  while (1) {
    if (Serial.available() > 0) {
      arquivo = Serial.readStringUntil('\n');
      Serial.println(arquivo);
      dataFile = SD.open(arquivo);
      Serial.println("==================================================");
      Serial.print("dados presentes no arquivo ");
      Serial.println(arquivo);
      if (dataFile) {
        while (dataFile.available()) {  // if the file is available, write to it:
          Serial.write(dataFile.read());
        }
        dataFile.close();
        Serial.println("==================================================");
      } else {  //se o arquivo não abrir, mostre a mensagem :
        msgErroSD(arquivo);
      }
      Serial.println("==================================================");
      break;
    }  //end if Serial avaliable
  }    //end while
}  // end ler
//============================================================

void apagar() {

  String arquivo;
  Serial.println("Digite o nome do arquivo com extencao ");
  while (1) {
    if (Serial.available() > 0) {
      arquivo = Serial.readStringUntil('\n');
      Serial.println(arquivo);
      if (!SD.remove(arquivo)) {
        Serial.print("não foi possivel apagar ");
        Serial.println(arquivo);
      } else {
        Serial.print("Arquivo ");
        Serial.print(arquivo);
        Serial.println(" apagado com sucesso!");
      }
      Serial.println("==================================================");
      break;
    }  //end if Serial avaliable
  }    //end while
}  // end apagar
//============================================================

void matrizT() {
  Serial.println("==================================================");
  Serial.println("dados presentes na matriz Temperatura ");
  for (uint16_t i = 0; i < sizeof(matrizExibicao) / sizeof(matrizExibicao[0]); i++) {
    Serial.print("indice ");
    Serial.print(i);
    Serial.print(" =");
    Serial.println(matrizExibicao[i]);
  }
  Serial.println("==================================================");
}  // end matrizT
//============================================================
/*
void matrizU() {
  Serial.println("==================================================");
  Serial.print("dados presentes na matriz Umidade ");
  for (uint16_t i = 0; i < (sizeof(matrizUmidade) / sizeof(matrizUmidade[0])); i++) {
    Serial.print("indice ");
    Serial.print(i);
    Serial.print(" =");
    Serial.println(matrizUmidade[i]);
  }
  Serial.println("==================================================");
}  // end matrizU
//============================================================
void matrizP() {
  Serial.println("==================================================");
  Serial.print("dados presentes na matriz Pressao ");
  for (uint16_t i = 0; i < (sizeof(matrizPressao) / sizeof(matrizPressao[0])); i++) {
    Serial.print("indice ");
    Serial.print(i);
    Serial.print(" =");
    Serial.println(matrizPressao[i]);
  }
  Serial.println("==================================================");
}  // end matrizP
*/
//============================================================
