char* menu1(bool opcao) {
  MenuName(BLUE_MENU, WHITE, "Motivo Parada"); // Nome Dos Menus (Cor de Fundo / Cor do Texto / Texto)
  if (opcao == false) {
    btn10.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Cliente", 2);
    btn11.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Refeicao", 2);
    btn12.initButton(&tft,  120, 154, 260, 40, WHITE, WHITE, BLACK, "Acidente", 2);
    btn13.initButton(&tft,  120, 200, 260, 40, WHITE, WHITE, BLACK, "Mecanico", 2);
    btn14.initButton(&tft,  120, 246, 260, 40, WHITE, WHITE, BLACK, "Policia", 2);
    btn15.initButton(&tft,  120, 292, 260, 40, WHITE, WHITE, BLACK, "Mais Opcoes", 2);
    btn10.drawButton(false);
    btn11.drawButton(false);
    btn12.drawButton(false);
    btn13.drawButton(false);
    btn14.drawButton(false);
    btn15.drawButton(false);
  } else {
    btn16.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Carga/Descarga", 2);
    btn17.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Banheiro", 2);
    btn18.initButton(&tft,  120, 154, 260, 40, WHITE, WHITE, BLACK, "Combustivel", 2);
    btn19.initButton(&tft,  120, 200, 260, 40, WHITE, WHITE, BLACK, "Malote", 2);
    btnvoltar.initButton(&tft,  120, 292, 260, 40, WHITE, WHITE, BLACK, "Voltar", 2);
    btnvoltar.drawButton(false);
    btn16.drawButton(false);
    btn17.drawButton(false);
    btn18.drawButton(false);
    btn19.drawButton(false);
  }
  return "menu1";
}
