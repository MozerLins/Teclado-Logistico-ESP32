void menu(bool opcao) {
  MenuName(BLUE_MENU, WHITE, "Menu Principal"); // Nome Dos Menus (Cor de Fundo / Cor do Texto / Texto);
  if (opcao == false) {

    btn1.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Inicio de Operacao", 2);
    btn2.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Reinicio Operacao", 2);
    btn3.initButton(&tft,  120, 154, 260, 40, WHITE, WHITE, BLACK, "Apoio Operacional", 2);
    btn4.initButton(&tft,  120, 200, 260, 40, WHITE, WHITE, BLACK, "Destravar", 2);
    btn5.initButton(&tft,  120, 246, 260, 40, WHITE, WHITE, BLACK, "Parada", 2);
    btn6.initButton(&tft,  120, 292, 260, 40, WHITE, WHITE, BLACK, "Mais Opcoes", 2);
    btn1.drawButton(false);
    btn2.drawButton(false);
    btn3.drawButton(false);
    btn4.drawButton(false);
    btn5.drawButton(false);
    btn6.drawButton(false);
  } else {
    btn7.initButton(&tft,  120, 62, 260, 40, WHITE, WHITE, BLACK, "Finalizar Operacao", 2);
    btn8.initButton(&tft,  120, 108, 260, 40, WHITE, WHITE, BLACK, "Emergencia", 2);
    btn9.initButton(&tft,  120, 154, 260, 40, WHITE, WHITE, BLACK, "Configuracao", 2);
    btnvoltar.initButton(&tft,  120, 292, 260, 40, WHITE, WHITE, BLACK, "Voltar", 2);
    btnvoltar.drawButton(false);
    btn7.drawButton(false);
    btn8.drawButton(false);
    btn9.drawButton(false);
  }
}
