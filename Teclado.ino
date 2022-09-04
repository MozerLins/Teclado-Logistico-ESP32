#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>
#include <BluetoothSerial.h>          // Biblioteca para o Bluetooh
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include "splashart.h" // Carrega Logo (Splash Art)

//---------------------------Configurações Iniciais-----------------------------------//
const int coords[] = {600, 3800, 3800, 700};           // portrait - left, right, top, bottom
const int rotation = 0;                               // Modo Retrato : rotation order - portrait, landscape, etc
const int XP = 27, XM = 15, YP = 4, YM = 14;         // Pino Padraão ESP32 Uno touchscreen pins
int item = 0;                                       // Define Menu Inicial
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Define TouchScreen como TS e Define As Pinasgens Do Touch
uint16_t pixel_x, pixel_y;                        //Variaveis de Coordenadas X / Y
const int bpon = 1;                              //Variavel para ligar ou desligar o Beep
String received;                                //Variavel para Salvar o Que Vem Da Serial BT
String ACK ;
char* menu_active;                             //Variavel para Identificação do Menu();

//---------------------------Flags do Sistema(15 Flags Totais)---------------------------------//
int Flag_Operacao = 0;
int Flag_Parada = 0;
int Flag_Carona = 0;
int Flag_Area_Verificacao = 0;
int Flag_ContraSenha = 0;
int Flag_Bau = 0;
int Flag_Ignicao = 0;
int Flag_VerificacaoArea = 0;
int Flag_Portas = 0;
int Flag_OperacaoBloqueada = 0;
int Flag_AreaMalote = 0;
int Flag_Pause = 0;
//int Flag_While = 0;
//int Flag_15 = 0;
//------------------------------BUFFER DE MENSAGEM------------------------------//
//ESTATICO//
/* Usado para dimensionar o array usado para armazenar as mensagens. O espaço disponível
será na verdade um a menos que isso, então 999. */
#define STORAGE_SIZE_BYTES 1000 
/* Define a memória que irá conter as mensagens dentro da mensagem
amortecedor. Deve ser um a mais que o valor passado no xBufferSizeBytes
parâmetro. */
static uint8_t ucStorageBuffer[ STORAGE_SIZE_BYTES ];

/* A variável usada para manter a estrutura do buffer de mensagem. */
StaticMessageBuffer_t xMessageBufferStruct;


//------------------------------Tarefeas do Sitesma----------------------------//
TaskHandle_t xHandle;
TaskHandle_t HandleKeep;
TaskHandle_t HandleConnect;

void KeepAlive(void *pvParameters);
void ReadSerial(void *pvParameters);
void Connect(void *pvParameters);

//----------------------------Configiração Bluetooh---------------------------------//
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//----------------------------Variaveis Globais Bluetooh---------------------------//
BluetoothSerial SerialBT;     // Atribui objeto SerialBT
const char *pin = "0000";    // Chave padrão do VL8
bool connected = false;      // Guarda o estado da conexao buletooth
char* name;               // Guarda o nome do Bluetooh

//----------------------------Ajuste de Pressão Na Tela---------------------------//
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
                    btn_ok, btn_cancel, btn_option1, btn_option2;

//-------------------Array De Botões-----------------------//
Adafruit_GFX_Button *buttons[] = {&btnprincipal, &btn1, &btn2, &btn3, &btn4, &btn5, &btn6, NULL};
Adafruit_GFX_Button *buttonsex[] = {&btnprincipal, &btnvoltar, &btn7, &btn8, &btn9, NULL};
Adafruit_GFX_Button *buttons1[] = {&btnprincipal, &btn10, &btn11, &btn12, &btn13, &btn14, &btn15, NULL};
Adafruit_GFX_Button *buttons1ex[] = {&btnprincipal, &btnvoltar, &btn16, &btn17, &btn18, &btn19, NULL};
Adafruit_GFX_Button *buttons2[] = {&btnprincipal, &btn20, &btn21, &btn22, &btn23, &btn24, NULL};
Adafruit_GFX_Button *buttons3[] = {&btnprincipal, &btn25, &btn26, NULL};
Adafruit_GFX_Button *numbers[] = {&btnprincipal, &num_1, &num_2, &num_3, &num_4, &num_5, &num_6, &num_7, &num_8, &num_9, &num_0, &num_del, &num_enter, NULL}; // Teclado Numerico
Adafruit_GFX_Button *confirm_buttons[] = {&btnprincipal, &btn_ok, &btn_cancel, NULL};
Adafruit_GFX_Button *alert_buttons[] = {&btnprincipal, &btn_ok, NULL};
Adafruit_GFX_Button *options[] = {&btn_option1, &btn_option2, NULL};

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
  if (b->justReleased()) {
    b->drawButton(false);
    delay(100);
  }
  if (b->justPressed()) {
    Beep(5);
    b->drawButton(true);

  }
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

void FailCommand() {
  if (!btnprincipal.justPressed()) {
    Alert(1, D, "FALHA" , "ENVIO DE COMANDO", 0);
    ReturnMenu(menu_active);
  }
}

void setup(void) {
  Serial.begin(115200);
  xTaskCreate(Connect, "Connect", 2048, NULL, 0, &HandleConnect);
//  BufferMensage();
  idDisplay(); //IDDisplay

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
  tft.setRotation(rotation);  // Define a Orientação
  Splash(BLACK, 200, 144, 1, BLACK); // Cores, resolução w x h / Para Alteração da imagem de splash deve-se Carregar na variavel global splashart a sequencia de binarios
  // name = "";
  SerialBT.register_callback(callback);  // Função de desconexão
  //SerialBT.setPin("0000");                 // Seta o pin do VL8 para conexão Padrão 0000
  SerialBT.begin("GW Terminal", true); // Define o nome do Bluetooh do teclado
  //  conectaBT();                        // Faz a primeira conexão com o bluetooh
  tft.fillScreen(BLACK);
  menu_active = menu(false);
  btnprincipal.initButton(&tft,  20, 341, 30, 20, WHITE, WHITE, BLACK, "", 1);
  btnprincipal.drawButton(false);
  xTaskCreate(KeepAlive, "KeepAlive", 2048, NULL, 2, &HandleKeep);
  xTaskCreate(ReadSerial, "ReadSerial", 2048, NULL, 1, &xHandle);

}

void loop() {
  if (Flag_Pause == 0) {
    if (Flag_OperacaoBloqueada == 0) {
      if (btnprincipal.justPressed()) {
        tft.fillScreen(BLACK);
        item = 0;
        menu_active = menu(false);
      }

      if (item == 0) {
        update_button_list(buttons);

        if (btn1.justPressed()) {
          if (Flag_Operacao == 0 and Flag_Portas == 0 and Flag_Ignicao == 1) {
            SerialBT.print(KeyboardNum("Digite o CPF", 11, "STP05"));
            if (connected == true) {
              SerialBT.print(KeyboardNum("Digite a Linha", 14, "STP06"));
              if (connected == true) {
                SerialBT.print(KeyboardNum("Digite o Tel", 11, "STP07"));
                if (connected == true) {
                  bool confirma = Alert(0, W, "ATENCAO", "Iniciar Operacao ?", 0);
                  tft.fillScreen(BLACK);
                  if (confirma == true and connected == true) {
                    SerialBT.print("SSH011");
                    if (connected == true) {
                      Alert(1, S, "SUCESSO" , "Operacao Iniciada", 0);
                      Flag_Operacao = 1;
                      Beep(100); delay(200); Beep(100);
                      tft.fillScreen(BLACK);
                      ReturnMenu(menu_active);
                    } else {
                      FailCommand();
                    }
                  } else {
                    FailCommand();
                  }
                } else {
                  FailCommand();
                }
              } else {
                FailCommand();
              }
            } else {
              FailCommand();
            }
          } //else if (Flag_Ignicao == 0) {
           // Alert(1, D, "IGNICAO" , "DESLIGADA", 0);
            //ReturnMenu(menu_active);
          //}
        }

        if (btn2.justPressed()) {
          if (Flag_Operacao == 1 and Flag_Parada == 1 and Flag_Portas == 0 and Flag_Bau == 0 and Flag_Ignicao == 0) {
            bool confirma = Alert(0, W, "ATENCAO" , "Reiniciar Operacao", 0);
            tft.fillScreen(BLACK);

            if (confirma == true) {
              SerialBT.print("SSH021");
              if (connected == true) {
                Alert(1, S, "SUCESSO" , "Operacao Iniciada", 0);
                Flag_Parada = 0;
                tft.fillScreen(BLACK);
                ReturnMenu(menu_active);
              } else {
                FailCommand();
              }
            } else {
              ReturnMenu(menu_active);
            }
          }
        }

        if (btn3.justPressed()) { // Apoio Operacional
          SerialBT.print(KeyboardNum("Digite o Tel", 11, "STP07"));
          if (!btnprincipal.justPressed()) {
            bool confirma = Alert(0, W, "ATENCAO", "Realizar Apoio ?", 0);
            if (confirma == true) {
              SerialBT.print("SCT04 6");
              if (connected == true) {
                Alert(1, S, "SUCESSO" , "Apoio Realizado", 0);
                ReturnMenu(menu_active);
              } else {
                FailCommand();
              }
            } else {
              ReturnMenu(menu_active);
            }
          }
        }
        if (btn4.justPressed()) {

        }

        if (btn5.justPressed()) { // Parada
          if (Flag_Operacao == 0 and Flag_Ignicao == 0) {
            if (strcmp(KeyboardNum("Informe a Senha", 6, "STP05"), "STP05 222222") == 0) {
              //if (connected == true) {
              Alert(1, S, "SUCESSO" , "Escolha Motivo", 0);
              tft.fillScreen(BLACK);
              menu_active =  menu1(false);
              item = 1;
              //} else {
              //  FailCommand();
              //  }
            } else {
              Alert(1, S, "FALHA" , "Senha Incorreta", 0);
              tft.fillScreen(BLACK);
              ReturnMenu(menu_active);
            }
          }
        }

        if (btn6.justPressed()) {
          tft.fillScreen(BLACK);
          menu_active = menu(true);

          while (true) {
            if (Flag_Pause == 1) {
              break;
            }
            update_button_list(buttonsex);

            if (btn7.justPressed()) { // Finalizar Operacao
              if (Flag_Operacao == 1 and Flag_Ignicao == 0 and Flag_Portas == 0 and Flag_Parada) {
                bool confirma = Alert(0, W, "ATENCAO", "Finalizar Operacao ?", 0);
                tft.fillScreen(BLACK);
                if (confirma == true) {
                  SerialBT.print("SSH121");
                  if (connected == true) {
                    Alert(1, S, "SUCESSO" , "Operacao Finalizada", 0);
                    Flag_Operacao = 0;
                    Beep(100); delay(200); Beep(100);
                    tft.fillScreen(BLACK);
                    ReturnMenu(menu_active);
                    break;
                  } else {
                    FailCommand();
                    break;
                  }
                } else {
                  tft.fillScreen(BLACK);
                  ReturnMenu(menu_active);
                  break;
                }
              }
            }
            if (btn8.justPressed()) { //Emergencia
              tft.fillScreen(BLACK);
              item = 2;
              menu_active = menu2();
              break;
            }
            if (btn9.justPressed()) { // Configuração
            }
            if (btnprincipal.justPressed()) {
              tft.fillScreen(BLACK);
              break;
            }
            if (btnvoltar.justPressed()) {
              tft.fillScreen(BLACK);
              ReturnMenu(menu_active);
              break;
            }
          }
        }
      }

      if (item == 1) { //Menu Paradas
        update_button_list(buttons1);
        if (btn10.justPressed()) { // Cliente
          int opcao = Options(W, "ATENCAO", "Parada Cliente");
          if (opcao == 1) {
            if (Alert(0, W, "ATENCAO", "Confirmar Operacao", 0) == true) {
              SerialBT.print("SCT03 1");
              if (connected == true) {
                if (Alert(3, W, "PORTA DO CARONA", "Abra a Porta Carona", Flag_Carona) == true)  {
                  if (Alert(2, W, "PORTA ABERTA", "Porta Carona Aberta", 0) == true) {
                    if (Alert(1, W, "DESTRAVAR BAU", " Aperte Ok", 0) == true) {
                      if (Alert(0, W, "DESTRAVAR BAU", "Confirmar Operação", 0) == true) {
                        if (connected == true) {
                          SerialBT.print("SSH091");
                          Flag_Parada = 1;
                        } else {
                          FailCommand();
                        }
                      }
                      else {
                        ReturnMenu(menu_active);
                      }
                    }
                  }
                } else {
                  ReturnMenu(menu_active);
                }
              } else {
                FailCommand();
              }
            } else {
              ReturnMenu(menu_active);
            }
          }
          else if (opcao == 2) {
            if (Alert(0, W, "ATENCAO", "Confirmar Operacao", 0) == true) {
              SerialBT.print("SCT03 7");
              if (connected == true) {
                if (Alert(3, W, "PORTA DO CARONA", "Abra a Porta Carona", Flag_Carona) == true)  {
                  if (Alert(2, W, "PORTA ABERTA", "Porta Carona Aberta", 0) == true) {
                    if (Alert(1, W, "DESTRAVAR BAU", " Aperte Ok", 0) == true) {
                      if (Alert(0, W, "DESTRAVAR BAU", "Confirmar Operação", 0) == true) {
                        if (connected == true) {
                          SerialBT.print("SSH091");
                          Flag_Parada = 1;
                        } else {
                          FailCommand();
                        }
                      }
                      else {
                        ReturnMenu(menu_active);
                      }
                    }
                  }
                } else {
                  ReturnMenu(menu_active);
                }
              } else {
                FailCommand();
              }
            } else {
              ReturnMenu(menu_active);
            }
          }
        }

        if (btn11.justPressed()) { // Refeicao
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?", 0);
          tft.fillScreen(BLACK);
          if (confirma == true) {
            SerialBT.print("SCT03 2");
            if (connected == true) {
              Alert(1, S, "SUCESSO" , "Parada Realizada", 0);
              Flag_Parada = 1;
              Beep(100); delay(200); Beep(100);
              tft.fillScreen(BLACK);
              menu_active = menu1(false);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }

        }
        if (btn12.justPressed()) { // Acidente
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?", 0);
          tft.fillScreen(BLACK);
          if (confirma == true) {
            SerialBT.print("SCT03 3");
            if (connected == true) {
              Alert(1, S, "SUCESSO" , "Parada Realizada", 0);
              Flag_Parada = 1;
              Beep(100); delay(200); Beep(100);
              tft.fillScreen(BLACK);
              menu_active = menu1(false);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }

        }

        if (btn13.justPressed()) { // Mecanico
          bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?", 0);
          tft.fillScreen(BLACK);
          if (confirma == true) {
            SerialBT.print("SCT03 4");
            if (connected == true) {
              Alert(1, S, "SUCESSO" , "Parada Realizada", 0);
              Flag_Parada = 1;
              Beep(100); delay(200); Beep(100);
              tft.fillScreen(BLACK);
              menu_active = menu1(false);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }

        if (btn14.justPressed()) {  // Policia  // Voltar Aqui
          if (Alert(0, W, "ATENCAO", "Confirmar Operacao", 0) == true) {
            if (connected == true) {
              SerialBT.print("SCT03 5");
              if (Alert(3, W, "PORTA DO CARONA", "Abra a Porta Carona", Flag_Carona) == true)  {
                if (Alert(2, W, "PORTA ABERTA", "Porta Carona Aberta", 0) == true) {
                  if (Alert(1, W, "DESTRAVAR BAU", " Aperte Ok", 0) == true) {
                    if (Alert(0, W, "DESTRAVAR BAU", "Confirmar Operação", 0) == true) {
                      if (connected == true) {
                        SerialBT.print("SSH091");
                        Flag_Parada = 1;
                      } else {
                        FailCommand();
                      }
                    }
                    else {
                      ReturnMenu(menu_active);
                    }
                  }
                }
              } else {
                ReturnMenu(menu_active);
              }
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn15.justPressed()) { //Mais Opções
          tft.fillScreen(BLACK);
          menu1(true);

          while (true) {
            update_button_list(buttons1ex);
            if (btn16.justPressed()) { // Carga e Descarga
              if (Alert(0, W, "ATENCAO", "Confirmar Operacao", 0) == true) {
                SerialBT.print("SCT03 6");
                if (connected == true) {
                  if (Alert(3, W, "PORTA DO CARONA", "Abra a Porta Carona", Flag_Carona) == true)  {
                    if (Alert(2, W, "PORTA ABERTA", "Porta Carona Aberta", 0) == true) {
                      if (Alert(1, W, "DESTRAVAR BAU", " Aperte Ok", 0) == true) {
                        if (Alert(0, W, "DESTRAVAR BAU", "Confirmar Operação", 0) == true) {
                          SerialBT.print("SSH091");
                          Flag_Parada = 1;
                        }
                        else {
                          ReturnMenu(menu_active);
                          break;
                        }
                      }
                    }
                  } else {
                    ReturnMenu(menu_active);
                    break;
                  }
                } else {
                  FailCommand();
                  break;
                }
              } else {
                ReturnMenu(menu_active);
                break;
              }
            }
            if (btn17.justPressed()) { // Banheiro
              bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?", 0);
              tft.fillScreen(BLACK);
              if (confirma == true) {
                if (connected == true) {
                  SerialBT.print("SCT03 9");
                  Alert(1, S, "SUCESSO" , "Parada Realizada", 0);
                  Flag_Parada = 1;
                  Beep(100); delay(200); Beep(100);
                  tft.fillScreen(BLACK);
                  menu1(false);
                } else {
                  FailCommand();
                  break;
                }
              } else {
                ReturnMenu(menu_active);
                break;
              }
            }
            if (btn18.justPressed()) { // Combustivel
              bool confirma = Alert(0, W, "ATENCAO", "Confirmar Parada ?", 0);
              tft.fillScreen(BLACK);
              if (confirma == true) {
                SerialBT.print("SCT03 10");
                if (connected == true) {
                  Alert(1, S, "SUCESSO" , "Parada Realizada", 0);
                  Flag_Parada = 1;
                  Beep(100); delay(200); Beep(100);
                  tft.fillScreen(BLACK);
                  menu1(false);
                } else {
                  FailCommand();
                  break;
                }
              } else {
                ReturnMenu(menu_active);
                break;
              }
            }
            if (btn19.justPressed()) { // Malote
              if (Alert(0, W, "ATENCAO", "Confirmar Operacao", 0) == true) {
                SerialBT.print("SCT03 8");
                if (connected == true) {
                  if (Alert(3, W, "PORTA DO CARONA", "Abra a Porta Carona", Flag_Carona) == true)  {
                    if (Alert(2, W, "PORTA ABERTA", "Porta Carona Aberta", 0) == true) {
                      if (Alert(1, W, "DESTRAVAR BAU", " Aperte Ok", 0) == true) {
                        if (Alert(0, W, "DESTRAVAR BAU", "Confirmar Operação", 0) == true) {
                          SerialBT.print("SSH091");
                          Flag_Parada = 1;
                          Beep(100); delay(200); Beep(100);
                          ReturnMenu(menu_active);
                          break;
                        }
                        else {
                          ReturnMenu(menu_active);
                          break;
                        }
                      }
                    }
                  } else {
                    ReturnMenu(menu_active);
                    break;
                  }
                } else {
                  FailCommand();
                  break;
                }
              } else {
                ReturnMenu(menu_active);
                break;
              }
            }
            if (btnprincipal.justPressed()) {

              tft.fillScreen(BLACK);
              break;
            }
            if (btnvoltar.justPressed()) {

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
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT04 1");
            if (connected == true) {
              Beep(100); delay(200); Beep(100);
              ReturnMenu(menu_active);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn21.justPressed()) {
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT04 2");
            if (connected == true) {
              Beep(100); delay(200); Beep(100);
              ReturnMenu(menu_active);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn22.justPressed()) {
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT04 3");
            if (connected == true) {
              Beep(100); delay(200); Beep(100);
              ReturnMenu(menu_active);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn23.justPressed()) {
          SerialBT.print(KeyboardNum("Digite o Tel", 11, "STP07"));
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT04 5");
            if (connected == true) {
              Beep(100); delay(200); Beep(100);
              ReturnMenu(menu_active);
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn24.justPressed()) {
          tft.fillScreen(BLACK);
          item = 3;
          menu_active = menu3();
        }
      }
      if (item == 3) {
        update_button_list(buttons3);
        if (btn25.justPressed()) {
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT08 0");
            if (connected == true) {
              SerialBT.print("SSH031");
              if (connected == true) {
                SerialBT.print("QCT08");
                if (connected == true) {
                  Alert(1, W, "CONTRA SENHA", "Em Breve", 0);
                  SerialBT.print(KeyboardNum("Contra Senha", 11, "SCT09"));
                  SerialBT.print("SSH041");
                  if (connected == true) {
                    SerialBT.print("SCC09 3");
                    if (connected == true) {
                      delay(5000);
                      if (Flag_ContraSenha ==  1) { // Senha Correta
                        SerialBT.print("SSXP11");
                        if (connected == true) {
                          SerialBT.print("SSH051");
                          Beep(100); delay(200); Beep(100);
                        } else {
                          FailCommand();
                        }
                        ReturnMenu(menu_active);
                      } else { // Senha Incorreta
                        Alert(1, W, "CONTRA SENHA", "Senha Incorreta", 0);
                        ReturnMenu(menu_active);
                      }

                    } else {
                      FailCommand();
                    }
                  } else {
                    FailCommand();
                  }
                } else {
                  FailCommand();
                }
              } else {
                FailCommand();
              }
            } else {
              FailCommand();
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
        if (btn26.justPressed()) {
          if (Alert(0, D, "ATENCAO", "Confirmar Emergencia", 0) == true) {
            SerialBT.print("SCT08 0");
            SerialBT.print("SSH031");
            SerialBT.print("QCT08");
            Alert(1, W, "CONTRA SENHA", "Em Breve", 0);
            SerialBT.print(KeyboardNum("Contra Senha", 11, "SCT09"));
            SerialBT.print("SSH041");
            SerialBT.print("SCC09 3");
            if (Flag_ContraSenha ==  1) { // Senha Correta
              SerialBT.print("SSH190");
              SerialBT.print("SSH051");
            } else if (Flag_ContraSenha == 2) { // Senha Incorreta
              Alert(1, W, "CONTRA SENHA", "Senha Incorreta", 0);
              ReturnMenu(menu_active);
            }
          } else {
            ReturnMenu(menu_active);
          }
        }
      }
    }
    if (Flag_OperacaoBloqueada == 1) {
      update_button_list(buttons2);
    }
  }
}
