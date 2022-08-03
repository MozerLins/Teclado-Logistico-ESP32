void menu3() {
  MenuName(BLUE_MENU, WHITE, "Contra Senha"); // Nome Dos Menus (Cor de Fundo / Cor do Texto / Texto);
  btn25.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Trava de Bau", 2);
  btn26.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Bloqueio de Veiculo", 2);
  btn25.drawButton(false);
  btn26.drawButton(false);
}
