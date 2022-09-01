void KeepAlive(void *pvParameters) {
  for (;;) {
    vTaskSuspend(xHandle);
    if (connected)
    {
      SerialBT.print("SSH001");                 // Keep Alive
      delay(3000);
      if (SerialBT.available()) {
        received =  SerialBT.readString();
        int posicao = received.indexOf(';');
        if (posicao >= 0) {
          received = received.substring(1, posicao);
          Serial.println(received);
          vTaskResume(xHandle);
        }
        received = "";
      } else {
        conectaBT();
      }
      }
      //vTaskResume(xHandle);
      vTaskDelay(30000);
    }
  }
