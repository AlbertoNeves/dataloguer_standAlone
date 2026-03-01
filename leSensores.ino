void leSensores() {
  digitalWrite(onOffBME, LOW);
  delay(10);
  bme.begin(0x76);
  delay(10);
  

/*
  sensors.requestTemperatures();
  temperaturaAtual = sensors.getTempCByIndex(0);
*/

  temperaturaAtual = bme.readTemperature()+ offSetTemp;
  umidadeAtual = bme.readHumidity();
  pressaoAtual = (bme.readPressure() / 100.0F);
  digitalWrite(onOffBME, HIGH);
  Serial.print("Temperatura | ");
  Serial.print("Umidade | ");
  Serial.println("Pressao | ");
  Serial.print("   ");
  Serial.print(temperaturaAtual);
  Serial.print("    | ");
  Serial.print(" ");
  Serial.print(umidadeAtual);
  Serial.print("  | ");
  Serial.print(pressaoAtual);
  Serial.println(" | ");
  Serial.println("=================================");

  //le tensao bateria
  tensaoBateria = analogRead(pinoBateria);
}  //end leSensores
