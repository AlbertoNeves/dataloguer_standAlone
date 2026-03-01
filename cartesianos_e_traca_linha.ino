
void traca_cartesiano() {


  uint16_t tempAgora = (uint16_t)temperaturaAtual * 10;
  if (tempAgora > tempAnterior) {
    tempAnterior = tempAgora;
    tempSai = tempAgora;
  }
  if (tempAgora < (tempAnterior - (10 * multiplicadorEscalaY))) {
    tempAnterior = tempAgora;
    tempSai = tempAgora + (10 * multiplicadorEscalaY);
  }
  escala_Y_min = tempSai - (10 * multiplicadorEscalaY);
  escala_Y_max = tempSai + (10 * multiplicadorEscalaY);
  escalaY_Mx_Mn = (escala_Y_max + escala_Y_min) / 20;  // media entre MAX e MIN da escala Y--esse 20 representa as posicoes disponiveis

  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.setDrawColor(1);

  uint8_t tracinhoX = (originX - 2);                       // para fazer o tracinho no eixo X
  uint8_t qtde_marcasY = 10;                               // quantidade de marcas(tracos) no eixo  X
  uint8_t dist_marcasY = ((originY - 13) / qtde_marcasY);  // menos 13 para caber dentro do intervlo de 53 a 13 (40 posicoes)
  uint8_t tracinhoY = (originY + 2);                       // para fazer o tracinho no eixo Y
  u8g2.drawLine(originX, originY, originX, 13);            //tracando eixo Y

  for (uint8_t i = 0; i < qtde_marcasY + 1; i++) {  // tracando as marcas e valores no eixo Y

    u8g2.drawLine(originX, originY - (dist_marcasY * i), tracinhoX, originY - (dist_marcasY * i));  //tracinhos do eixo Y

    u8g2.setCursor(0, originY - (dist_marcasY * i) + 3);  // colocando os valores no eixo Y
    if (i == 0) {
      u8g2.print(escalaY_Mx_Mn - (1 * multiplicadorEscalaY));  // colocando valor inferior
    } else if (i == qtde_marcasY / 2) {
      u8g2.print(escalaY_Mx_Mn);  // colocando valor central
    } else if (i == qtde_marcasY) {
      u8g2.print(escalaY_Mx_Mn + (1 * multiplicadorEscalaY));  // colocando valor superior
    }
  }  //end for tracando as marcas e valores  no eixo Y
     //==============================================
     //tracando eixo X
  uint8_t qtde_marcasX = 6;

  uint8_t dist_marcasX = (tamanhoX - originX) / qtde_marcasX;
  u8g2.drawLine(originX, originY, tamanhoX, originY);  //tracando eixo X

  for (uint8_t i = 0; i < qtde_marcasX + 1; i++) {                                                  // tracando as marcas no eixo X
    u8g2.drawLine(originX + (dist_marcasX * i), originY, originX + (dist_marcasX * i), tracinhoY);  //colocando marcas
    if (valorX + i >= 10) {
      u8g2.setCursor(originX + (dist_marcasX * i - 4), tamanhoY);  //posicionando cursor para colocar valor
    } else {
      u8g2.setCursor(originX + (dist_marcasX * i - 2), tamanhoY);  //posicionando cursor para colocar valor
    }
    u8g2.print(valorX + i);  // colocando valores
  }                          //end for tracando as marcas no eixo X
}  // end traca_cartesiano
//-------------------------------------------------------------------------

void traca_Linha() {
  uint8_t acertaPixel = 0;
  uint16_t pixel;
  uint16_t inicioJ2 = 0;
  uint16_t fimJ2 = 120;

  if (controlaIndiceMatriz == 0) {
    if (matrizExibicao[120] == 0) {
      inicioJ2 = 0;
      fimJ2 = 119;
      valorX = 0;
    } else if (matrizExibicao[240] == 0) {
      inicioJ2 = 120;
      fimJ2 = 239;
      valorX = 6;
    } else if (matrizExibicao[360] == 0) {
      inicioJ2 = 240;
      fimJ2 = 359;
      valorX = 12;
    } else if (matrizExibicao[360] != 0) {
      inicioJ2 = 360;
      fimJ2 = 479;
      valorX = 18;
    }
  } else if (controlaIndiceMatriz == 1) {
    inicioJ2 = 0;
    fimJ2 = 119;
    valorX = 0;
  } else if (controlaIndiceMatriz == 2) {
    inicioJ2 = 20;
    fimJ2 = 139;
    valorX = 1;
  } else if (controlaIndiceMatriz == 3) {
    inicioJ2 = 40;
    fimJ2 = 159;
    valorX = 2;
  } else if (controlaIndiceMatriz == 4) {
    inicioJ2 = 60;
    fimJ2 = 179;
    valorX = 3;
  } else if (controlaIndiceMatriz == 5) {
    inicioJ2 = 80;
    fimJ2 = 199;
    valorX = 4;
  } else if (controlaIndiceMatriz == 6) {
    inicioJ2 = 100;
    fimJ2 = 219;
    valorX = 5;
  } else if (controlaIndiceMatriz == 7) {
    inicioJ2 = 120;
    fimJ2 = 239;
    valorX = 6;
  } else if (controlaIndiceMatriz == 8) {
    inicioJ2 = 140;
    fimJ2 = 259;
    valorX = 7;
  } else if (controlaIndiceMatriz == 9) {
    inicioJ2 = 160;
    fimJ2 = 279;
    valorX = 8;
  } else if (controlaIndiceMatriz == 10) {
    inicioJ2 = 180;
    fimJ2 = 299;
    valorX = 9;
  } else if (controlaIndiceMatriz == 11) {
    inicioJ2 = 200;
    fimJ2 = 319;
    valorX = 10;
  } else if (controlaIndiceMatriz == 12) {
    inicioJ2 = 220;
    fimJ2 = 339;
    valorX = 11;
  } else if (controlaIndiceMatriz == 13) {
    inicioJ2 = 240;
    fimJ2 = 359;
    valorX = 12;
  } else if (controlaIndiceMatriz == 14) {
    inicioJ2 = 260;
    fimJ2 = 379;
    valorX = 13;
  } else if (controlaIndiceMatriz == 15) {
    inicioJ2 = 280;
    fimJ2 = 399;
    valorX = 14;
  } else if (controlaIndiceMatriz == 16) {
    inicioJ2 = 300;
    fimJ2 = 419;
    valorX = 15;
  } else if (controlaIndiceMatriz == 17) {
    inicioJ2 = 320;
    fimJ2 = 439;
    valorX = 16;
  } else if (controlaIndiceMatriz == 18) {
    inicioJ2 = 340;
    fimJ2 = 459;
    valorX = 17;
  } else if (controlaIndiceMatriz == 19) {
    inicioJ2 = 360;
    fimJ2 = 479;
    valorX = 18;
  } else if (controlaIndiceMatriz >= 20) {
    controlaIndiceMatriz = 19;
  }


  for (uint16_t j2 = inicioJ2; j2 <= fimJ2; j2++) {
    pixel = map(matrizExibicao[j2], escala_Y_max, escala_Y_min, 13, originY);  //esse 13 corresponde ao topo da area de ploltagem
    if ((pixel >= originY) || (pixel <= 13)) {                                 // esse valor representa ZERO e MAXIMO na escala de plotagem
      pixel = 130;                                                             //fazendo assim, todo valor que for MENOR que a MINIMA ou MAIOR que a MAXIMA escala     abaixo do ultimo pixel visivel
    }
    acertaPixel++;
    u8g2.drawPixel(originX + acertaPixel, pixel);
  }  // end for  j2

/*
  Serial.println("===================================");
  Serial.print("escala Maxima ");
  Serial.println(escala_Y_max);
  Serial.println("===================================");
  Serial.print("escala Minima ");
  Serial.println(escala_Y_min);
  Serial.println("===================================");*/

}  // end void traca_Linha
