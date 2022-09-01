//------------------- Função para desconexão do Bluetooh-----------------------//
void Connect(void *pvParameters) {
  for (;;) {
    if (connected == false) {
      conectaBT();
    }
    vTaskDelay(50);
  }
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_CLOSE_EVT)
  {
    Beep(5);
    Serial.println("Desconectado");
    // tft.fillScreen(BLACK);
    //Alert(2, S, "BLUETOOTH" , "RECONECTANDO...", 0);
    // ReturnMenu(menu_active);
    //delay(1000);
    connected = false;
    //conectaBT();
  }
}
//-----------------------------------------------------------------------------//

//----------- Estabelece Conexão com com VL8 através do bluetooth-------------//
void conectaBT()
{
  //Alert(2, S, "AGUARDE" , "Conectando BT...");
  Serial.println("Conectando BT...");
  connected = SerialBT.connect("VIRTEC_VL8_0102");
  if (connected)
  {
    //delay(100);
    //Alert(2, S, "SUCESSO" , "Conectado");
    Serial.println("Connected Succesfully!");
    //SerialBT.print("PFL,btcon");
    //Serial.println("Conectado");
    Beep(5);
  }
  else
  {
    Serial.println("Falha conexao");
    delay(2000);
    connected = false;

  }
}
//-----------------------------------------------------------------------------//
