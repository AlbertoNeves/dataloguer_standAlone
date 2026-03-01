/*      
        selecao = 0;//     exibicao numerica da temperatura , erro cartao , estado carga bateria.....
        selecao = 1;//     DisplayDateTime()--mostra hora/data
        selecao = 2;//     exibicao grafica das 6 ultimas horas da temperatura
        selecao = 3;//     rolagem horizontal da temperatura - acescimo ou decrescimo 1 hora na escala de 6 horas da tempereatura 
        selecao = 4;//     faz voltar para selecao ZERO
        selecao = 5;//     menu (itens abaixo)
        selecao = 6;//     ajusta relogio
        selecao = 7;//     Offset temperatura
        selecao = 8;//     lista arquivos no cartao sd que combinam com a escolha acima (mantem em memoria os nomes- avanca automatico)
        selecao = 9;//     grafico bateria
        selecao = 10;//    use up ou down ate arquivo escolhido ficar iuminado, use home(botao do meio) para exibir.   use teclas para avancar ou retroceder e aumentar escala 
        20 segundos sem apertar botao , volta para selecao 0
    */
//---------------------------------------------------------


void checaBotao() {

  home.leitura();

  if (selecao != 5) {
    RightNext.leitura();
    LeftPrev.leitura();
  }
  Up.leitura();
  Down.leitura();

  if (home.foiPressionada() || RightNext.foiPressionada() || LeftPrev.foiPressionada() || Up.foiPressionada() || Down.foiPressionada()) {
    tela = 0;
    tempON = 0;
    brilho = EEPROM.read(10);
    analogWrite(pinBackLight, brilho);

  }  // libera tela para uma exibição por rolagem
  //-------------------------------------------------
  if (home.pressionadaPor(2000)) {  // Se a home  foi pressionada por 2s , entra menu
    selecao = 5;                    //vai para menu
  }                                 //end (home.pressionadaPor
  //-------------------------------------------------

  if (home.foiPressionada()) {

    switch (selecao) {
      case 5:
        switch (posiCursor) {
          case 1:  //("Ajusta Relogio");
            selecao = 6;
            break;
          case 2:  //" Offset temperatura ");
            selecao = 7;
            break;
          case 3:  //(" mostrar arquivos do SD ");
            selecao = 8;
            break;
          case 4:  //" grafico tensao bateria
            selecao = 9;
            break;
          case 5:  //"sair");
            selecao = 0;
            break;
        }       //end posiCursor
        break;  // end case 5
      case 6:
        AdjClockFuncao++;
        break;  //end case 6
      case 7:
        EEPROM.updateFloat(30, offSetTemp);
        leSensores();
        selecao = 0;
        break;  //end case 7
      case 8:
        selecao = 0;
        break;  //end case 8
      case 9:
        selecao = 0;
        break;  //end break9
      default:
        tempON = 0;
        brilho = EEPROM.read(10);
        analogWrite(pinBackLight, brilho);
        selecao = 0;
        conta_tempo = 0;
        controlaIndiceMatriz = 0;
        break;  //end acao padrao para botao
    }           // endswitch (selecao)
  }             //end home.foiPressionada
  //-------------------------------------------------
  if (RightNext.foiPressionada()) {
    tempON = 0;
    conta_tempo = 0;

    switch (selecao) {
      case 3:
        controlaIndiceMatriz++;
        if (controlaIndiceMatriz >= 20) { controlaIndiceMatriz = 19; }

        if (controlaIndiceMatriz == 0) {
          if (valorX == 0) {
            controlaIndiceMatriz = 1;
          } else if (valorX == 6) {
            controlaIndiceMatriz = 7;
          } else if (valorX == 12) {
            controlaIndiceMatriz = 13;
          } else if (valorX == 18) {
            controlaIndiceMatriz = 19;
          }
        }  //end if (controlaIndiceMatriz == 0)
        break;
      default:
        selecao++;
        break;
    }  //end  switch (selecao)
  }    //end RightNext.foiPressionada
  //-------------------------------------------------
  if (LeftPrev.foiPressionada()) {
    tempON = 0;
    conta_tempo = 0;

    switch (selecao) {
      case 3:

        controlaIndiceMatriz--;
        if (controlaIndiceMatriz <= 1) { controlaIndiceMatriz = 1; }


        if (controlaIndiceMatriz == 0) {
          if (valorX == 0) {
            controlaIndiceMatriz = 1;
          } else if (valorX == 6) {
            controlaIndiceMatriz = 7;
          } else if (valorX == 12) {
            controlaIndiceMatriz = 13;
          } else if (valorX == 18) {
            controlaIndiceMatriz = 19;
          }
        }  //end if (controlaIndiceMatriz == 0)
        break;

      default:
        selecao--;
        break;
    }  // end  switch (selecao)
  }    //end LeftPrev.foiPressionada
  //-------------------------------------------------
  if (Up.foiPressionada()) {
    tempON = 0;
    conta_tempo = 0;
    switch (selecao) {
      case 3:
        multiplicadorEscalaY++;

        multiplicadorEscalaY = constrain(multiplicadorEscalaY, 1, 6);
        controlaIndiceMatriz = constrain(controlaIndiceMatriz, 1, 19);

        break;
      case 7:
        offSetTemp = offSetTemp + 0.1;
        conta_tempo = 0;
        break;
      case 5:
        posiCursor--;  // decrementa posiCursor para movimentar cursor de escolha
        break;
      case 6:
        switch (AdjClockFuncao) {  // ajusta hora +
          case 0:
            if (horaUpdate == 23) {
              horaUpdate = 0;
            } else {
              horaUpdate = horaUpdate + 1;
            }
            break;  // end AdjClockFuncao==0 e horaUpdate
          case 1:   // ajusta minuto +
            if (minUpdate == 59) {
              minUpdate = 0;
            } else {
              minUpdate = minUpdate + 1;
            }
            break;  //end AdjClockFuncao==1 e minUpdate
          case 2:   // ajusta dia +
            if (diaUpdate == 31) {
              diaUpdate = 1;
            } else {
              diaUpdate = diaUpdate + 1;
            }
            break;  //end AdjClockFuncao==2 e diaUpdate
          case 3:   // ajusta mes +
            if (mesUpdate == 12) {
              mesUpdate = 1;
            } else {
              mesUpdate = mesUpdate + 1;
            }
            break;  //end AdjClockFuncao==3 e mesUpdate
          case 4:   // ajusta ano +
            anoUpdate = anoUpdate + 1;
            break;  //end AdjClockFuncao==4 e anoUpdate
        }           //endswitch (AdjClockFuncao)
        break;      // end selecao == 6
    }               // end switch selecao
  }                 //end Up.foiPressionada
  //-------------------------------------------------
  if (Down.foiPressionada()) {
    tempON = 0;
    conta_tempo = 0;
    switch (selecao) {
      case 3:
        multiplicadorEscalaY--;
        multiplicadorEscalaY = constrain(multiplicadorEscalaY, 1, 6);
        controlaIndiceMatriz = constrain(controlaIndiceMatriz, 1, 19);

        break;
      case 7:
        offSetTemp = offSetTemp - 0.1;
        conta_tempo = 0;
        break;
      case 5:
        posiCursor++;  // incremente posiCursor para movimentar cursor de escolha
        break;
      case 6:
        switch (AdjClockFuncao) {
          case 0:  // ajusta hora -
            if (horaUpdate == 0) {
              horaUpdate = 23;
            } else {
              horaUpdate = horaUpdate - 1;
            }
            break;  // end case 0 e horaUpdate
          case 1:   // ajusta minuto -
            if (minUpdate == 0) {
              minUpdate = 59;
            } else {
              minUpdate = minUpdate - 1;
            }
            break;  //end case 1 e minUpdate
          case 2:   // ajusta dia -
            if (diaUpdate == 1) {
              diaUpdate = 31;
            } else {
              diaUpdate = diaUpdate - 1;
            }
            break;  //end case 2 e diaUpdate
          case 3:   // ajusta mes -
            if (mesUpdate == 1) {
              mesUpdate = 12;
            } else {
              mesUpdate = mesUpdate - 1;
            }
            break;  //end case 3 e mesUpdate
          case 4:   // ajusta ano  -
            anoUpdate = anoUpdate - 1;
            break;  //end case 4 e anoUpdate
        }           // end switch (AdjClockFuncao)
        break;      // end case 6
    }               // end switch (selecao)
  }                 //end Down.foiPressionada
  //-------------------------------------------------

  //-------------------------------------------------

  //-------------------------------------------------

  menu();


}  // end checaBotao