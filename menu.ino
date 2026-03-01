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
        selecao = 9;//     Grafico Bateria
        selecao = 10;//    use up ou down ate arquivo escolhido ficar iuminado, use home(botao do meio) para exibir.   use teclas para avancar ou retroceder e aumentar escala 
        20 segundos sem apertar botao , volta para selecao 0
    */
void menu() {
  switch (selecao) {
    case 0:                // exibe data, hora, status cartao SD, status bateria
      u8g2.clearBuffer();  //zera buffer da tela de exibicao
      u8g2.drawRFrame(2, 2, 124, 60, 7);
      u8g2.setCursor(15, 13);
      u8g2.setFont(u8g2_font_courR08_tr);
      u8g2.print("Temperatura Atual");
      u8g2.setCursor(35, 48);
      u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
      //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
      u8g2.print(temperaturaAtual, 1);
      u8g2.print("");

      // icones do sistema ----- bateria ------

      u8g2.setCursor(93, 58);
      u8g2.setFont(u8g2_font_micro_tn);
      u8g2.print((tensaoBateria / 1023.0) * 5.0);
      /*
696
685
679
675
669
667
665
659
655
614
512
*/





      if (tensaoBateria >= 696) {              // maior que 696, 3,4V, indica 100%
        u8g2.setFont(u8g2_font_battery19_tn);  // fonte icone bateria
        u8g2.drawGlyph(110, 58, 53);           // posicao X, posicao Y, icone a ser exibido
      } else if (tensaoBateria >= 695) {       // maior que 695,	3,393685V,	70% carga= indica 80%
        u8g2.setFont(u8g2_font_battery19_tn);
        u8g2.drawGlyph(110, 58, 52);
      } else if (tensaoBateria >= 635) {  // maior que 635,	3,100705V, 	50% carga= indica 60%
        u8g2.setFont(u8g2_font_battery19_tn);
        u8g2.drawGlyph(110, 58, 51);
      } else if (tensaoBateria >= 575) {  // maior que 575,	2,807725V, 	30% de carga= indica 40%
        u8g2.setFont(u8g2_font_battery19_tn);
        u8g2.drawGlyph(110, 58, 50);
      } else if (tensaoBateria >= 515) {  // maior que 515,	2,514745V, 	10% de carga= indica 20%
        u8g2.setFont(u8g2_font_battery19_tn);
        u8g2.drawGlyph(110, 58, 49);
      } else if (tensaoBateria >= 485) {  // maior que 485,	2,368255V, 	0% de carga = indica 0%
        u8g2.setFont(u8g2_font_battery19_tn);
        u8g2.drawGlyph(110, 58, 48);
      } else if (tensaoBateria <= 485) {  // menor que 485,	2,368255V, 	0% de carga = pisca indicativo bateria
        if (onOff % 2 == 0) {
          u8g2.setFont(u8g2_font_battery19_tn);
          u8g2.drawGlyph(110, 58, 48);
        } else {
          u8g2.setFont(u8g2_font_battery19_tn);
          u8g2.drawGlyph(110, 58, 47);
        }
      }  // icones do sistema ----- bateria ------

      // icones do sistema ----- cartao ruim ------
      if (sdERRO == 1) {
        if (onOff % 2 == 0) {
          u8g2.setFont(u8g2_font_streamline_computers_devices_electronics_t);
          u8g2.drawGlyph(5, 58, 67);
        } else {
          u8g2.setFont(u8g2_font_streamline_computers_devices_electronics_t);
          u8g2.drawGlyph(5, 58, 78);
        }
      }                   // end if(sdERRO==1)
      u8g2.sendBuffer();  //envia para display
      break;
    //end case 0 ---------------------------------------------------------
    case 1:                // exibe data e hora
      u8g2.clearBuffer();  //zera buffer da tela de exibicao
      DisplayDateTime();
      u8g2.sendBuffer();  //envia para display
      break;
    //end case 1
    case 2:  //Temp.ultimas 6 Horas
      if (tela == 0) {
        u8g2.clearBuffer();  //zera buffer da tela de exibicao
        montaMatriz(nomeArquivoDia + ".tem");
        traca_cartesiano();
        traca_Linha();

        u8g2.drawFrame(9, 0, 119, 10);
        u8g2.setCursor(11, 8);
        u8g2.setFont(u8g2_font_courR08_tr);
        u8g2.print("Temp.ultimas 6 Horas");
        u8g2.sendBuffer();  //envia para display
        tela = 1;
      }
      break;
    //end case 2---------------------------------------------------------
    case 3:  // navega pelo grafico de temperatura
      if (tela == 0) {
        u8g2.clearBuffer();  //zera buffer da tela de exibicao
        montaMatriz(nomeArquivoDia + ".tem");
        traca_cartesiano();
        traca_Linha();

        u8g2.drawFrame(9, 0, 119, 10);
        u8g2.setCursor(11, 8);
        u8g2.setFont(u8g2_font_courR08_tr);
        u8g2.print(" use botoes ^  < >  ");
        u8g2.sendBuffer();  //envia para display
        tela = 1;
      }
      break;
    //end case 3---------------------------------------------------------
    case 4:  //retorna ao inicio
      //selecao = 0;
      break;
    //end case 4---------------------------------------------------------
    case 5:  // opcoes de menu
      {      //essa chave foi adicionada para eliminar um aviso de cruamento de inicializacao por causa do menuitem

        u8g2.clearBuffer();  //zera buffer da tela de exibicao
        u8g2.drawRFrame(2, 2, 124, 60, 7);
        if (posiCursor < 1) {
          posiCursor = 5;
        } else if (posiCursor >= 6) {
          posiCursor = 1;
        }
        switch (posiCursor) {
          case 1:                           // Ajusta Relogio
            u8g2.drawFrame(7, 6, 115, 10);  //selecao = 6
            break;
          case 2:                            // Offset temperatura
            u8g2.drawFrame(7, 17, 115, 10);  //selecao = 7
            break;
          case 3:                            // mostra arquivos no SD
            u8g2.drawFrame(7, 28, 115, 10);  //selecao = 8
            break;
          case 4:                            //" Grafico Bateria
            u8g2.drawFrame(7, 39, 115, 10);  //selecao = 9
            break;
          case 5:  //"sair");
            u8g2.drawFrame(7, 50, 115, 10);
            break;
        }  // end switch posiCursor

        const char *itensMenu[] = {
          "Ajusta Relogio", "Offset temperatura", "mostra arquivos SD", "Grafico Bateria ", "sair"
        };

        u8g2.setDrawColor(2);  //
        u8g2.setFont(u8g2_font_courR08_tr);
        u8g2.setCursor(14, 14);
        u8g2.print(itensMenu[0]);
        u8g2.setCursor(14, 25);
        u8g2.print(itensMenu[1]);
        u8g2.setCursor(14, 36);
        u8g2.print(itensMenu[2]);
        u8g2.setCursor(14, 47);
        u8g2.print(itensMenu[3]);
        u8g2.setCursor(14, 58);
        u8g2.print(itensMenu[4]);
        u8g2.setDrawColor(1);
        u8g2.sendBuffer();  //envia para display
                            //---------------------------------------------------------
        break;              //  case 5:
      }                     //essa chave foi adicionada para eliminar um aviso de cruamento de inicializacao por causa do menuitem
    //end case 5---------------------------------------------------------
    case 6:  // ajusta relogio

      if (AdjClockFuncao == 0) {  // ajustando hora + ou -
        u8g2.clearBuffer();       //zera buffer da tela de exibicao
        DisplaySetHour();
        u8g2.sendBuffer();  //envia para display
      }
      //-----------------------------------------------------
      if (AdjClockFuncao == 1) {  // ajustando minuto + ou -
        u8g2.clearBuffer();       //zera buffer da tela de exibicao
        DisplaySetMinute();
        u8g2.sendBuffer();  //envia para display
      }
      //-----------------------------------------------------
      if (AdjClockFuncao == 2) {  // ajustando dia + ou -
        u8g2.clearBuffer();       //zera buffer da tela de exibicao
        DisplaySetDay();
        u8g2.sendBuffer();  //envia para display
      }
      //---------------------------------------------------------
      if (AdjClockFuncao == 3) {  // ajustando mes + ou -
        u8g2.clearBuffer();       //zera buffer da tela de exibicao
        DisplaySetMonth();
        u8g2.sendBuffer();  //envia para display
      }
      //-----------------------------------------------------
      if (AdjClockFuncao == 4) {  // ajustando ano + ou -
        u8g2.clearBuffer();       //zera buffer da tela de exibicao
        DisplaySetYear();
        u8g2.sendBuffer();  //envia para display
      }
      //-----------------------------------------------------
      if (AdjClockFuncao == 5) {  // Grava alteracao de data e hora
        RTC.adjust(DateTime(anoUpdate, mesUpdate, diaUpdate, horaUpdate, minUpdate, 30));
        selecao = 0;
        AdjClockFuncao = 0;
      }
      break;
    //end case 6 ---------------------------------------------------------
    case 7:                // sOffset temperatura
      u8g2.clearBuffer();  //zera buffer da tela de exibicao
      u8g2.drawRFrame(2, 2, 124, 60, 7);
      u8g2.setFont(u8g2_font_courR08_tr);
      u8g2.setCursor(5, 23);
      u8g2.print(" USE BOTAO INC,DEC");
      u8g2.setCursor(40, 48);
      u8g2.setFont(u8g2_font_crox5hb_tn);
      u8g2.print(offSetTemp);
      u8g2.sendBuffer();  //envia para display
      break;
      //end case 7 ---------------------------------------------------------
    case 8:                  //mostrar arquivos do SD
      {                      //essa chave foi adicionada para eliminar um aviso de cruamento de inicializacao
        u8g2.clearBuffer();  //zera buffer da tela de exibicao
        u8g2.setFont(u8g2_font_courR08_tr);
        uint8_t entre_linhas = 8;  //para fazer rolagem dos nomes

        File root;
        root = SD.open("/");  // Abrir a raiz do cartão SD
        while (true) {
          File entry = root.openNextFile();
          u8g2.setCursor(1, entre_linhas);
          u8g2.print(entry.name());
          u8g2.sendBuffer();  //envia para display
          entre_linhas += 8;
          delay(500);

          if (entre_linhas >= 60) {
            entre_linhas = 8;
            u8g2.clearBuffer();
            delay(1000);
          }
          if (!entry) {
            // no more files
            selecao = 0;
            entry.close();
            //indice = 0;
            delay(1000);
            break;
          }
          entry.close();
        }
        break;
      }  //essa chave foi adicionada para eliminar um aviso de cruamento de inicializacao
    //end case 8 ---------------------------------------------------------
    case 9:  // Grafico Bateria

     
      if (tela == 0) {
        u8g2.clearBuffer();  //zera buffer da tela de exibicao
        montaMatriz(nomeArquivoDia + ".bat");
        traca_Linha();
        traca_cartesiano();
        u8g2.drawFrame(9, 0, 119, 10);
        u8g2.setCursor(11, 8);
        u8g2.setFont(u8g2_font_courR08_tr);
        u8g2.print("carga e descarga bateria");
        u8g2.sendBuffer();  //envia para display
        Serial.println("arquivo a ler");
        tela = 1;
      }
      break;
      //end case 9 ---------------------------------------------------------
    default:
      Serial.print("default case");
      break;  //end acao padrao para botao
  }           //end switch selecao
}