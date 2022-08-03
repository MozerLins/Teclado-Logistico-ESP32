void menu2() {
  MenuName(BLUE_MENU, WHITE, "Menu Emergencia"); // Nome Dos Menus (Cor de Fundo / Cor do Texto / Texto);
  btn20.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Perseguicao", 2);
  btn21.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Carga Exposta", 2);
  btn22.initButton(&tft,  120, 154, 260, 40, WHITE, WHITE, BLACK, "Roubo Consumado", 2);
  btn23.initButton(&tft,  120, 200, 260, 40, WHITE, WHITE, BLACK, "Chamar Central", 2);
  btn24.initButton(&tft,  120, 246, 260, 40, WHITE, WHITE, BLACK, "Contrassenha", 2);
  btn20.drawButton(false);
  btn21.drawButton(false);
  btn22.drawButton(false);
  btn23.drawButton(false);
  btn24.drawButton(false);
}
