void Error(byte erro) {

  switch (erro)  {
    case 0:
      Serial.println(" RTC ruim, ou nao funcionando");

      break;

    case 1:
      Serial.println(" SENSOR BME nao funcionando");

      break;

    case 2:
      //Serial.println(" cartao ruim, ou nao inserido");
      sdERRO = 1;
      Serial.print("Tentando Reabrir cartao SD ");
      if (!SD.begin(CS_SD))
      {
        Serial.println(" cartao ruim, ou nao inserido");
        Serial.println("=============================");
      }
      else {
        Serial.println(" cartao inicializado.");
        Serial.println("=============================");
        sdERRO = 0;
      }
      break;
  }//end switch erro

}//end void Error
//-------------------------------------------------------
void msgErroSD(String nomeArquivo) {
  // se o arquivo não abrir, mostre a mensagem :
  Serial.println ("");
  Serial.print("Arquivo invalido ou Erro ao abrir ");
  Serial.print(nomeArquivo);
  Serial.println ("");

  Serial.print("Tentando Reabrir cartao SD ");
  if (!SD.begin(CS_SD))
  {
    Serial.println(" cartao ruim, ou nao inserido");
  }
  else {
    Serial.println(" cartao inicializado.");
  }
}//end void msgErroSD
