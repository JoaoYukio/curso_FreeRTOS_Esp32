#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>

#define LED 2
#define BOT 12

SemaphoreHandle_t semaforo;
TaskHandle_t xTaskHandleBot;

void vTaskBotao(void* pvParameters);
void ISR_Callback()
{
  BaseType_t xHighPriorityTaskWoken = pdTRUE; 
  xSemaphoreGiveFromISR(semaforo, &xHighPriorityTaskWoken);
  if(xHighPriorityTaskWoken==pdTRUE)
  {
    portYIELD_FROM_ISR();
  }

}

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(BOT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOT), ISR_Callback, FALLING);

  semaforo = xSemaphoreCreateBinary();
  xTaskCreate(vTaskHandleBot, "TaskBotão", configMINIMAL_STACK_SIZE + 1024, NULL, 5, &xTaskHandleBot);

}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(300));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTaskBotao(void* pvParameters)
{
  int contador =0;
  for(;;){
    if(xSemaphoreTake(semaforo, portMAX_DELAY))
    {
      Serial.println("x: "+ String(contador++));
    }
  }
}
