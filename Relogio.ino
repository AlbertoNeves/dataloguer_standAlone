/*
   Ajustado por Alberto em outubro 2019
*/


void DisplayDateTime() {
  // Mostramos a data e hora atuais
  DateTime now = RTC.now();
  
 

  u8g2.drawRFrame(1, 2, 126, 60, 7);

  u8g2.setCursor(3, 28);

  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  if (now.hour() <= 9) {
    u8g2.print("0");
  }
  u8g2.print(now.hour(), DEC);
  horaUpdate = now.hour();
  u8g2.print(":");
  if (now.minute() <= 9) {
    u8g2.print("0");
  }
  u8g2.print(now.minute(), DEC);
  minUpdate = now.minute();
  u8g2.print(":");
  if (now.second() <= 9) {
    u8g2.print("0");
  }
  u8g2.print(now.second(), DEC);


  u8g2.setCursor(3, 58);
  if (now.day() <= 9) {
    u8g2.print("0");
  }
  u8g2.print(now.day(), DEC);
  diaUpdate = now.day();
  u8g2.print("/");
  if (now.month() <= 9) {
    u8g2.print("0");
  }
  u8g2.print(now.month(), DEC);
  mesUpdate = now.month();
  u8g2.print("/");
  u8g2.print(now.year() - 2000);
  anoUpdate = now.year();
}  // end void display date time
//--------------------------------------------
void DisplaySetHour() {
  DateTime now = RTC.now();
  u8g2.drawRFrame(1, 2, 125, 60, 7);
  u8g2.setCursor(15, 13);
  u8g2.setFont(u8g2_font_courR08_tr);
  u8g2.print("  Ajusta Hora:  ");
  u8g2.setCursor(pos_x, 46);
  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  if (horaUpdate <= 9) {
    u8g2.print("0");
  }
  u8g2.print(horaUpdate);

}  // end void DisplaySetHour
//----------------------------------------------

void DisplaySetMinute() {
  u8g2.drawRFrame(1, 2, 125, 60, 7);
  u8g2.setCursor(15, 13);
  u8g2.setFont(u8g2_font_courR08_tr);
  u8g2.print(" Ajusta Minutos:");
  u8g2.setCursor(pos_x, 46);
  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  if (minUpdate <= 9) {
    u8g2.print("0");
  }
  u8g2.print(minUpdate);

}  // end void DisplaySetMinute
//----------------------------------------------
void DisplaySetYear() {
  u8g2.drawRFrame(1, 2, 125, 60, 7);
  u8g2.setCursor(15, 13);
  u8g2.setFont(u8g2_font_courR08_tr);
  u8g2.print("  Ajusta Ano:   ");
  u8g2.setCursor(pos_x - 20, 46);
  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  u8g2.print(anoUpdate);

}  // end void DisplaySetYear
//----------------------------------------------
void DisplaySetMonth() {
  u8g2.drawRFrame(1, 2, 125, 60, 7);
  u8g2.setCursor(15, 13);
  u8g2.setFont(u8g2_font_courR08_tr);
  u8g2.print("  Ajusta Mes:   ");
  u8g2.setCursor(pos_x, 46);
  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  if (mesUpdate <= 9) {
    u8g2.print("0");
  }
  u8g2.print(mesUpdate);

}  // end void DisplaySetMonth
//----------------------------------------------
void DisplaySetDay() {
  u8g2.drawRFrame(1, 2, 125, 60, 7);
  u8g2.setCursor(15, 13);
  u8g2.setFont(u8g2_font_courR08_tr);
  u8g2.print("  Ajusta dia:   ");
  u8g2.setCursor(pos_x, 46);
  u8g2.setFont(u8g2_font_timB24_tn);  // para display azul, letras brancas
  //u8g2.setFont(u8g2_font_maniac_tn);// para display verde, letras pretas
  if ((diaUpdate) <= 9) {
    u8g2.print("0");
  }
  u8g2.print(diaUpdate);

}  // end void DisplaySetDay
   //----------------------------------------------
