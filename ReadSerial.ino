void ACK_Send() {
  String Envio = "ACK;ID=0102;";
  Envio = Envio += ACK;
  SerialBT.print(Envio);
}

void ReadSerial(void *pvParameters) {
  for (;;) {
    if (SerialBT.available()) {
      received = SerialBT.readString();

      int ACK_VERIFY = received.indexOf('#');
      if (ACK_VERIFY >= 0) {
        ACK = received.substring(ACK_VERIFY, ACK_VERIFY + 5);
      }

      int posicao = received.indexOf(';');
      if (posicao >= 0) {
        received = received.substring(1, posicao);
      }
      Serial.println(received);

      ACK_Send();

      if (received == "SUS2101" and Flag_Ignicao == 0) {  //IGN LIGADA
        Flag_Ignicao = 1;


      } else if (received == "SUS2100" and Flag_Ignicao == 1) { // IGN DESLIGADA
        Flag_Ignicao = 0;

      } else if (received == "SUS2600" and Flag_Portas == 0) { //ALGUMA PORTA ABERTA
        Flag_Portas = 1;


      } else if (received == "SUS2601" and Flag_Portas == 1) { //TODA PORTAS FECHADAS
        Flag_Portas = 0;


      } else if (received == "SUS2801" and Flag_OperacaoBloqueada == 0) { //OPERACAO BLOQUEADA
        Flag_Pause = 1;
        Flag_OperacaoBloqueada = 1;
        tft.fillScreen(BLACK);
        Alert(2, W, "ATENCAO", "OPERACAO BLOQUEADA", 0);
        tft.fillScreen(BLACK);
        item = 2;
        menu_active = menu2();
        Flag_Pause = 0;

      } else if (received == "SUS2800" and Flag_OperacaoBloqueada == 1) { //OPERACAO DESBLOQUEADA
        Flag_Pause = 1;
        Flag_OperacaoBloqueada = 0;
        tft.fillScreen(BLACK);
        Alert(2, W, "ATENCAO", "OPERACAO DESBLOQUEADA", 0);
        tft.fillScreen(BLACK);
        item = 0;
        menu_active = menu(false);
        Flag_Pause = 0;
      } else if (received == "SUS2700" and  Flag_VerificacaoArea == 1) { // VERIF AREA OFF
        Flag_VerificacaoArea = 0;

      } else if (received == "SUS2701" and Flag_VerificacaoArea == 0) { // VERIF AREA OFF
        Flag_VerificacaoArea = 1;

      } else if (received == "SUS0100" and Flag_AreaMalote == 1) { // FORA DA AREA DE MALOTE
        Flag_AreaMalote = 0;

      } else if (received == "SUS0101" and Flag_AreaMalote == 0) { // FORA DA AREA DE MALOTE
        Flag_AreaMalote = 1;

      }
    }
    vTaskDelay(1000);
  }
}
