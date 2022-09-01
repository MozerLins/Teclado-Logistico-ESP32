// Função para exibir uma mensagem ou solicitar uma confirmação
//Parametros: int(opcao)
//Opcao :
//0 - Confirmação
//1 - Apresentar Mensagem Com Botão Ok
//2 - Apresentar Mensagem com Delay de 8 Segundos
//3 - Apresentar Mensagem e Aguarda a Flag estar Ativa
//Tipo = S - Sucesso(Azul) W -Warning(Amarelo)  D -Danger(Vermelho)
//Titulo = Titludo a ser Apresentado
//Mensagem = Texto a ser apresentado até 18 caracters
//Flag - Flag de Opção a Verificar

char temp_mensagem[18];
char temp_titulo[18];

bool Alert(int opcao, uint16_t tipo, char* titulo ,  char* mensagem, int flag)
{
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
  if (opcao == 0) {
    btn_ok.initButton(&tft,  62, 210, 92, 40, tipo, WHITE, BLACK, "Sim", 2);
    btn_cancel.initButton(&tft,  177, 210, 92, 40, tipo, WHITE, BLACK, "Nao", 2);
    btn_ok.drawButton(false);
    btn_cancel.drawButton(false);

    while (true) {
      update_button_list(confirm_buttons);
      if (btn_ok.justPressed()) {

        delay(50);
       // Flag_While = 0;
        return true;
      }
      if (btn_cancel.justPressed()) {

        delay(50);
//        Flag_While = 0;
        return false;
      }
      if (Flag_Pause == 1) {
        break;
      }
    }
  }
  else if (opcao == 1) {
    //Flag_While = 1;
    btn_ok.initButton(&tft,  120, 210, 92, 40, tipo, WHITE, BLACK, "OK", 2);
    btn_ok.drawButton(false);

    while (true) {
      if (Flag_Pause == 1) {
        break;
      }
      update_button_list(alert_buttons);
      if (btn_ok.justPressed()) {

        delay(50);
       // Flag_While = 0;
        return true;
      }
    }
  }
  else if (opcao == 2) {
    while (true) {
      if (Flag_Pause == 1) {
        break;
      }
      delay(8000);
      //Flag_While = 0;
      return true;
    }
  }
  else if (opcao == 3) {
    btn_ok.initButton(&tft,  120, 210, 92, 40, tipo, WHITE, BLACK, "Cancelar", 2);
    btn_ok.drawButton(false);

    while (true) {
      if (Flag_Pause == 1) {
        break;
      }
      update_button_list(alert_buttons);
      if (flag == 1) {
//        Flag_While = 0;
        return true;

      } else if (btn_ok.justPressed()) {
      //  Flag_While = 0;
        return false;
      }
    }
  }
}
