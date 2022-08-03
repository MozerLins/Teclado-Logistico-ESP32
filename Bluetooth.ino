//------------------- Função para desconexão do Bluetooh-----------------------//
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_CLOSE_EVT)
  {
    Beep(5);
    Serial.println("Desconectado");
    delay(1000);
    connected = true;
  }
}
//-----------------------------------------------------------------------------//

//----------- Estabelece Conexão com com VL8 através do bluetooth-------------//
void conectaBT()
{
  Serial.println("Conectando BT...");
  connected = SerialBT.connect("DESKTOP-SJV2C2N");
  if (connected)
  {
    delay(100);
    Serial.println("Connected Succesfully!");
    SerialBT.print("PFL,btcon");
    Serial.println("Conectado");
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
