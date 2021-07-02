#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED 2
#define BT 12

TaskHandle_t xTaskBThandle;

void vTaskTrataBT(void* pvParameters);

void callBackBT()
{
  vTaskNotifyGiveFromISR(xTaskBThandle, pdFALSE);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT), callBackBT, FALLING);
  xTaskCreate(vTaskTrataBT, "Botão", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTaskBThandle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTaskTrataBT(void* pvParameters)
{

  for(;;)
  {
    if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY))//o pdTrue diz que ele nao vai limpar o notify quando ele pegar um
    {
      Serial.println("Tratando ISR do botão!");
      digitalWrite(LED, !digitalRead(LED));
      delay(500);
    }
  }
}
