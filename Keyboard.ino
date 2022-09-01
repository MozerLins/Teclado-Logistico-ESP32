void c(char text[], char caracter) {
  size_t tamanho = strlen(text);
  if (tamanho) {
    text[tamanho] = caracter;
    text[tamanho + 1] = '\0';
  }
}

char* KeyboardNum(char* marker , int s , char* command) {
  char *input = (char*)malloc(18);
  input[0] = '\0';
  strcpy(input, command);
  strcat (input, " ");

  bool notext = true;
  int a = 40;
  int b = 40;
  int limit = 14;
  if (s <= 14) {
    limit = s * 12 + a;
  }
  tft.fillScreen(WHITE);
  tft.drawRoundRect(30, 20, 180, 60, 3, BLACK);
  num_1.initButton(&tft,  55, 110, 50, 50, BLACK, WHITE, BLACK, "1", 2);
  num_2.initButton(&tft,  120, 110, 50, 50, BLACK, WHITE, BLACK, "2", 2);
  num_3.initButton(&tft,  185, 110, 50, 50, BLACK, WHITE, BLACK, "3", 2);
  num_4.initButton(&tft,  55, 170, 50, 50, BLACK, WHITE, BLACK, "4", 2);
  num_5.initButton(&tft,  120, 170, 50, 50, BLACK, WHITE, BLACK, "5", 2);
  num_6.initButton(&tft,  185, 170, 50, 50, BLACK, WHITE, BLACK, "6", 2);
  num_7.initButton(&tft,  55, 230, 50, 50, BLACK, WHITE, BLACK, "7", 2);
  num_8.initButton(&tft,  120, 230, 50, 50, BLACK, WHITE, BLACK, "8", 2);
  num_9.initButton(&tft,  185, 230, 50, 50, BLACK, WHITE, BLACK, "9", 2);
  num_del.initButton(&tft,  55, 290, 50, 50, BLACK, WHITE, BLACK, "<-", 2);
  num_0.initButton(&tft,  120, 290, 50, 50, BLACK, WHITE, BLACK, "0", 2);
  num_enter.initButton(&tft,  185, 290, 50, 50, BLACK, WHITE, BLACK, "Ok", 2);
  num_1.drawButton(false);
  num_2.drawButton(false);
  num_3.drawButton(false);
  num_4.drawButton(false);
  num_5.drawButton(false);
  num_6.drawButton(false);
  num_7.drawButton(false);
  num_8.drawButton(false);
  num_9.drawButton(false);
  num_0.drawButton(false);
  num_del.drawButton(false);
  num_enter.drawButton(false);

  while (true) {
    update_button_list(numbers);

    if (btnprincipal.justPressed()) {
      tft.fillScreen(BLACK);
      item = 0;
      return "\0";

    }
    if (Flag_Pause == 1) {
      break;
    }

    if (a < limit) {
      if (a == 40) {
        tft.setTextColor(0x7c50);
        tft.setCursor(40 + (160 / 2) - (strlen(marker) * 3 * 2), 40);
        tft.print(marker);
        notext = true;
      }


      tft.setCursor (a, b);
      tft.setTextColor(BLACK);
      tft.setTextSize (2);

      if (num_1.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);
        }
        tft.print("1");
        a = a + 12;
        c(input, '1');
      }
      if (num_2.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);
        }
        tft.print("2");
        a = a + 12;
        c(input, '2');

      }
      if (num_3.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("3");
        a = a + 12;
        c(input, '3');

      }
      if (num_4.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("4");
        a = a + 12;
        c(input, '4');

      }
      if (num_5.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("5");
        a = a + 12;
        c(input, '5');

      }
      if (num_6.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("6");
        a = a + 12;
        c(input, '6');

      }
      if (num_7.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("7");
        a = a + 12;
        c(input, '7');

      }
      if (num_8.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("8");
        a = a + 12;
        c(input, '8');

      }
      if (num_9.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("9");
        a = a + 12;
        c(input, '9');

      }
      if (num_del.justPressed() and a > 40) {
        a = a - 12;
        tft.fillRect(a, b, 12, 20, WHITE);
        input[strlen(input) - 1] = '\0';

      }

      if (num_0.justPressed()) {
        if (notext == true) {
          notext = false;
          tft.fillRect(a - 8, b, 176, 20, WHITE);

        }
        tft.print("0");
        a = a + 12;
        c(input, '0');

      }

      if (num_enter.justPressed()) {
        Beep(15);
      }

    } else {
      if (num_enter.justPressed()) {
        //tft.fillScreen(BLACK);
        return input;

      }
      if (num_del.justPressed() and a > 40) {
        a = a - 12;
        tft.fillRect(a, b, 12, 20, WHITE);
        input[strlen(input) - 1] = '\0';

      }
    }
  }
}
