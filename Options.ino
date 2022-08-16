int Options(uint16_t tipo, char* titulo ,  char* mensagem) {
  strncpy(temp_titulo, titulo, 18); // Copiar um limite de caracters Char* para uma outra variavel Char (destino,variavel,limite)
  temp_titulo[18] = 0;// strncpy does not place a null at the end.
  // When 'label' is >20 characters, _label is not terminated.
  strncpy(temp_mensagem, mensagem, 18); // Copiar um limite de caracters Char* para uma outra variavel Char (destino,variavel,limite)
  temp_titulo[18] = 0;// strncpy does not place a null at the end.
  // When 'label' is >20 characters, _label is not terminated.

  tft.fillScreen(BLACK); // Limpa a Tela
  tft.fillRoundRect(5, 80, 230, 40, 3, tipo);
  tft.drawRoundRect(5, 80, 230, 40, 3, tipo);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(5 + (230 / 2) - (strlen(temp_titulo) * 3 * 2),
                80 + (30 / 2) - (4 * 2));
  tft.write(temp_titulo);

  tft.fillRoundRect(5, 110, 230, 130, 3, WHITE);
  tft.drawRoundRect(5, 110, 230, 130, 3, tipo);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(5 + (230 / 2) - (strlen(mensagem) * 3 * 2), 140);
  tft.write(mensagem);
  
  btn_option1.initButton(&tft,  62, 210, 92, 40, tipo, WHITE, BLACK, "Opcao 1", 2);
  btn_option2.initButton(&tft,  177, 210, 92, 40, tipo, WHITE, BLACK, "Opcao 2", 2);
  btn_option1.drawButton(false);
  btn_option2.drawButton(false);

  while (true) {
    update_button_list(options);

    if (btn_option1.justPressed()) {
      Beep(5);
      delay(50);
      return 1;
    }
    if (btn_option2.justPressed()) {
      Beep(5);
      delay(50);
      return 2;
    }
  }
}
