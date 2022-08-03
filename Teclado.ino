//#include <Adafruit_GFX.h>         // Core graphics library
#include <BluetoothSerial.h>          // Biblioteca para o Bluetooh
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include "splashart.h" // Carrega Logo

//Configurações Iniciais
const int coords[] = {600, 3800, 3800, 700};           // portrait - left, right, top, bottom
const int rotation = 0;                               // Modo Retrato : rotation order - portrait, landscape, etc
const int XP = 27, XM = 15, YP = 4, YM = 14;         // Pino Padraão ESP32 Uno touchscreen pins
int item = 0;                                       // Define Menu Inicial
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Define TouchScreen como TS e Define As Pinasgens Do Touch
uint16_t pixel_x, pixel_y;                        //Variaveis de Coordenadas X / Y
const int bpon = 1;                              //Variavel para ligar ou desligar o Beep
char* received;

//---------------Configiração Bluetooh-------------------//

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//----------------------------Variaveis Globais Bluetooh---------------------------//
BluetoothSerial SerialBT;     // Atribui objeto SerialBT
const char *pin = "0000";    // Chave padrão do VL8
bool connected;      // Guarda o estado da conexao buletooth
String name;               // Guarda o nome do Bluetooh
//--------------Ajuste de Pressão Na Tela----------------//

#define MINPRESSURE 200
#define MAXPRESSURE 1000
//-------------------Cores Utilizadas-------------------//

#define BLACK   0x0000
#define WHITE   0xFFFF
#define BLUE_MENU   0x11CD
#define S 0x11CD // AZUL SUCESSO
#define W 0xD5C1 // AMARELO WARNING
#define D 0xD041 // VERMELHO DANGER
//-------------------Criar Botões-----------------------//
Adafruit_GFX_Button btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn10, btn11, btn12,
                    btn13, btn14, btn15, btn16, btn17, btn18, btn19, btn20, btn21, btn22, btn23,
                    btn24, btn25, btn26, btn27, btn28, btn29, btn30, btnvoltar, btnprincipal,
                    num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0, num_del, num_enter,
                    btn_ok, btn_cancel;

//-------------------Array De Botões-----------------------//
Adafruit_GFX_Button *buttons[] = {&btnprincipal, &btn1, &btn2, &btn3, &btn4, &btn5, &btn6, NULL};
Adafruit_GFX_Button *buttonsex[] = {&btnprincipal, &btnvoltar, &btn7, &btn8, &btn9, NULL};
Adafruit_GFX_Button *buttons1[] = {&btnprincipal, &btn10, &btn11, &btn12, &btn13, &btn14, &btn15, NULL};
Adafruit_GFX_Button *buttons1ex[] = {&btnprincipal, &btnvoltar, &btn16, &btn17, &btn18, &btn18, &btn19, NULL};
Adafruit_GFX_Button *buttons2[] = {&btnprincipal, &btn20, &btn21, &btn22, &btn23, &btn24, NULL};
Adafruit_GFX_Button *buttons3[] = {&btnprincipal, &btn25, &btn26, NULL};
Adafruit_GFX_Button *numbers[] = {&btnprincipal, &num_1, &num_2, &num_3, &num_4, &num_5, &num_6, &num_7, &num_8, &num_9, &num_0, &num_del, &num_enter, NULL}; // Teclado Numerico
Adafruit_GFX_Button *confirm_buttons[] = {&btnprincipal, &btn_ok, &btn_cancel, NULL};
Adafruit_GFX_Button *alert_buttons[] = {&btnprincipal, &btn_ok, NULL};
//-------------------Função dos Menus-----------------------//
//extern void menu(); // Principal
//extern void menu1(); // Menu Operação
//extern void menu2();
//extern void menu3();
//-----------Função para Confirmação de Telas---------------//

//-------------Função para Beep do Teclado------------------//
void Beep(int timer) {
  if (bpon == 1)                   // Ligar Beep se for igual a 1
  {
    pinMode(0, OUTPUT);         // define pino no Esp32 para o hardware de beep
    digitalWrite(0, LOW);      // ativa o pino Digital 32
    delay(timer);                 // espera por um segundo
    digitalWrite(0, HIGH);   // desativa o pino digital 32
  }
  else                      // desligar se for diferente de 1 ou seja : 0
  {
    pinMode(0, INPUT);// define pino no Esp32 para o hardware de beep
  }
}
//---------------------------------------------------------//
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();  //Pontos Selecionados No Touch X, Y e Z

  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);

  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);// p.z valor da pressão obtido da função GetPoint()

  if (pressed) {
    switch (rotation) {
      case 0: // portrait
        pixel_x = map(p.x, coords[0], coords[1], 0, tft.width());
        pixel_y = map(p.y, coords[2], coords[3], 0, tft.height());
        break;
      case 1: // landscape
        pixel_x = map(p.y, coords[1], coords[0], 0, tft.width());
        pixel_y = map(p.x, coords[2], coords[3], 0, tft.height());
        break;
      case 2: // portrait inverted
        pixel_x = map(p.x, coords[1], coords[0], 0, tft.width());
        pixel_y = map(p.y, coords[3], coords[2], 0, tft.height());
        break;
      case 3: // landscape inverted
        pixel_x = map(p.y, coords[0], coords[1], 0, tft.width());
        pixel_y = map(p.x, coords[3], coords[2], 0, tft.height());
        break;
    }
  }
  return pressed;
}

bool update_button(Adafruit_GFX_Button * b, bool pressed)
{
  b->press(pressed && b->contains(pixel_x, pixel_y));
  if (b->justReleased())
    b->drawButton(false);
  if (b->justPressed())
    b->drawButton(true);
  return pressed;
}

bool update_button_list(Adafruit_GFX_Button **pb)
{
  boolean pressed = Touch_getXY(); // Verifica Se Pressionado
  for (int i = 0 ; pb[i] != NULL; i++) {
    update_button(pb[i], pressed);
  }
  return pressed;
}
void Splash(uint16_t backgroundsplash, int w, int h, int tempo, uint16_t background) {
  tft.fillScreen(backgroundsplash); // Define Cor de Fundo
  tft.drawBitmap(20, 80, splashart, w, h, 65535); // coordenadas x,y / imagem em sequencia de byte / resolução w x h , cores
  delay(tempo * 1000); // Tempo De Splash
  tft.fillScreen(background);      // Define Cor de Fundo
}

void MenuName(uint16_t background, uint16_t textcolor, const char* title) {
  tft.fillRect(0, 1, 240, 35, background);
  tft.setCursor(40, 10);
  tft.setTextSize(2);
  tft.setTextColor(textcolor);
  tft.write(title);
}
void idDisplay() {
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
}

void setup(void) {
  Serial.begin(115200);
  // Beep(5);
  idDisplay(); //IDDisplay
  // name = "";
  //SerialBT.register_callback(callback);  // Função de desconexão
  //SerialBT.setPin();                 // Seta o pin do VL8 para conexão Padrão 0000

  // SerialBT.enableSSP();
  //SerialBT.begin("GW Terminal", true); // Define o nome do Bluetooh do teclado
  // conectaBT();                        // Faz a primeira conexão com o bluetooh

  //-------------------Orientação da Tela--------------------//
  String orientation;
  switch (rotation) {
    case 0:
      orientation = "Portrait";
      break;
    case 1:
      orientation = "Landscape";
      break;
    case 2:
      orientation = "Portrait Inverted";
      break;
    case 3:
      orientation = "Landscape Inverted";
      break;
  }
  //--------------------------------------------------------//
  idDisplay();
  tft.setRotation(rotation);  // Define a Orientação
  Splash(BLACK, 200, 144, 1, BLACK); // Cores, resolução w x h / Para Alteração da imagem de splash deve-se Carregar na variavel global splashart a sequencia de binarios
  menu(false);
  btnprincipal.initButton(&tft,  20, 341, 30, 20, WHITE, WHITE, BLACK, "", 1);
  btnprincipal.drawButton(false);
}

void loop() {

  
  if (btnprincipal.justPressed()) {
    tft.fillScreen(BLACK);
    item = 0;
    menu(false);
  }

  if (item == 0) {
    update_button_list(buttons);

    if (btn1.justPressed()) {
      Beep(5);
      delay(50);
      KeyboardNum("Digite o CPF", 11);
      KeyboardNum("Digite a Linha", 14);
      KeyboardNum("Digite o Tel", 11);
      bool confirma = Alert(0, W, "ATENCAO", "Iniciar Operacao ?");
      tft.fillScreen(BLACK);
      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Operacao Iniciada");
        Beep(100); delay(200); Beep(100);
        tft.fillScreen(BLACK);
        menu(false);
      } else {
        menu(false);
      }
    }

    if (btn2.justPressed()) {
      Beep(5);
      delay(50);
      bool confirma = Alert(0, W, "ATENCAO" , "Reiniciar Operacao");
      tft.fillScreen(BLACK);

      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Operacao Iniciada");
        tft.fillScreen(BLACK);
        menu(false);
      } else {
        menu(false);
      }

    }
    if (btn3.justPressed()) {
      Beep(5);
      delay(50);
    }
    if (btn4.justPressed()) {
      Beep(5);
      delay(50);

    }
    if (btn5.justPressed()) { // Parada
      Beep(5);
      //delay(50);
      received = KeyboardNum("Informe a Senha", 6);
      Serial.println(received);
    }
    if (btn6.justPressed()) {
      Beep(5);
      delay(50);
      tft.fillScreen(BLACK);
      menu(true);

      while (true) {
        update_button_list(buttonsex);
        if (btn7.justPressed()) {
          Beep(5);
          delay(50);
          bool confirma = Alert(0, W, "ATENCAO", "Finalizar Operacao ?");
          tft.fillScreen(BLACK);
          if (confirma == true) {
            Alert(1, S, "SUCESSO" , "Operacao Finalizada");
            Beep(100); delay(200); Beep(100);
            tft.fillScreen(BLACK);
            menu(false);
          } else {
            tft.fillScreen(BLACK);
            menu(false);
          }
        }
        if (btn8.justPressed()) {
          Beep(5);
          delay(50);
          tft.fillScreen(BLACK);
          item = 2;
          menu2();
          break;


        }
        if (btn9.justPressed()) {
          Beep(5);
          delay(50);
        }
        if (btnprincipal.justPressed()) {
          Beep(5);
          delay(50);
          tft.fillScreen(BLACK);
          break;
        }
        if (btnvoltar.justPressed()) {
          Beep(5);
          delay(50);
          tft.fillScreen(BLACK);
          menu(false);
          break;
        }
      }
    }
  }

  if (item == 1) {
    update_button_list(buttons1);

    if (btn10.justPressed()) { // Cliente
      Beep(5);
      delay(50);
    }
    if (btn11.justPressed()) { // Refeicao
      Beep(5);
      delay(50);
      bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
      tft.fillScreen(BLACK);
      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Parada Realizada");
        Beep(100); delay(200); Beep(100);
        tft.fillScreen(BLACK);
        menu1(false);
      } else {
        tft.fillScreen(BLACK);
        item = 0;
        menu(false);
      }

    }
    if (btn12.justPressed()) { // Acidente
      Beep(5);
      delay(50);
      bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
      tft.fillScreen(BLACK);
      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Parada Realizada");
        Beep(100); delay(200); Beep(100);
        tft.fillScreen(BLACK);
        menu1(false);
      } else {
        tft.fillScreen(BLACK);
        item = 0;
        menu(false);
      }

    }
    if (btn13.justPressed()) { // Mecanico
      Beep(5);
      delay(50);
      bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
      tft.fillScreen(BLACK);
      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Parada Realizada");
        Beep(100); delay(200); Beep(100);
        tft.fillScreen(BLACK);
        menu1(false);
      } else {
        tft.fillScreen(BLACK);
        item = 0;
        menu(false);
      }

    }
    if (btn14.justPressed()) {  // Policia
      Beep(5);
      delay(50);
      bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
      tft.fillScreen(BLACK);
      if (confirma == true) {
        Alert(1, S, "SUCESSO" , "Parada Realizada");
        Beep(100); delay(200); Beep(100);
        tft.fillScreen(BLACK);
        menu1(false);
      } else {
        tft.fillScreen(BLACK);
        item = 0;
        menu(false);
      }

    }
    if (btn15.justPressed()) { //Mais Opções
      Beep(5);
      delay(50);
      tft.fillScreen(BLACK);
      menu1(true);

      while (true) {
        update_button_list(buttons1ex);
        if (btn16.justPressed()) { // Carga e Descarga
          Beep(5);
          delay(50);
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
          tft.fillScreen(BLACK);
          if (confirma == true) {
            Alert(1, S, "SUCESSO" , "Parada Realizada");
            Beep(100); delay(200); Beep(100);
            tft.fillScreen(BLACK);
            menu1(false);
          } else {
            tft.fillScreen(BLACK);
            item = 0;
            menu(false);
          }
        }
        if (btn17.justPressed()) { // Banheiro
          Beep(5);
          delay(50);
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
          tft.fillScreen(BLACK);
          if (confirma == true) {
            Alert(1, S, "SUCESSO" , "Parada Realizada");
            Beep(100); delay(200); Beep(100);
            tft.fillScreen(BLACK);
            menu1(false);
          } else {
            tft.fillScreen(BLACK);
            item = 0;
            menu(false);
          }
        }
        if (btn18.justPressed()) { // Combustivel
          Beep(5);
          delay(50);
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
          tft.fillScreen(BLACK);
          if (confirma == true) {
            Alert(1, S, "SUCESSO" , "Parada Realizada");
            Beep(100); delay(200); Beep(100);
            tft.fillScreen(BLACK);
            menu1(false);
          } else {
            tft.fillScreen(BLACK);
            item = 0;
            menu(false);
          }
        }
        if (btn19.justPressed()) { // Malote
          Beep(5);
          delay(50);
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?");
          tft.fillScreen(BLACK);
          if (confirma == true) {
            Alert(1, S, "SUCESSO" , "Parada Realizada");
            Beep(100); delay(200); Beep(100);
            tft.fillScreen(BLACK);
            menu1(false);
          } else {
            tft.fillScreen(BLACK);
            item = 0;
            menu(false);
          }
        }
        if (btnprincipal.justPressed()) {
          Beep(5);
          delay(50);
          tft.fillScreen(BLACK);
          break;
        }
        if (btnvoltar.justPressed()) {
          Beep(5);
          delay(50);
          tft.fillScreen(BLACK);
          menu1(false);
          break;
        }

      }
    }
  }
  if (item == 2) {
    update_button_list(buttons2);
    if (btn20.justPressed()) {
      Beep(5);
      delay(50);
    }
    if (btn21.justPressed()) {
      Beep(5);
      delay(50);
    }
    if (btn22.justPressed()) {
      Beep(5);
      delay(50);
    }
    if (btn23.justPressed()) {
      Beep(5);
      delay(50);
    }
    if (btn24.justPressed()) {
      Beep(5);
      delay(50);
      tft.fillScreen(BLACK);
      item = 3;
      menu3();
    }
  }
  if (item == 3) {
    update_button_list(buttons3);
  }
}

// Função Verificação Pressão
