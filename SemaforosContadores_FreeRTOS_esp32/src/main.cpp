#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 2
#define BT 12

SemaphoreHandle_t semaforoContador;
TaskHandle_t taskTrataBThandle;

void vTaskTrataBT(void* pvParameters);

void ISR_Callback()// uma interrupção para "gerenciar" o semaforo( adicionar no semaforo)
{
  BaseType_t xHighPriorityTaskWoken = pdTRUE;
  xSemaphoreGiveFromISR(semaforoContador, &xHighPriorityTaskWoken);// quando acontece a interrução ela giva para o semaforo contador

  if(xHighPriorityTaskWoken == pdTRUE)
  {
    portYIELD_FROM_ISR();
  }

}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BT,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT), ISR_Callback, FALLING);

  //Criação do semaforo e da task
  semaforoContador = xSemaphoreCreateCounting(255, 0);//(contagem, valor inicial)
  xTaskCreate(vTaskTrataBT, "Task trata botao", configMINIMAL_STACK_SIZE + 1024, NULL, 3, &taskTrataBThandle);


}

void loop() {//uma task para rodar em pararelo piscando o led da placa
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(500));
}

void vTaskTrataBT(void* pvParameters)// uma task para tratar o botao
{
  int x = 0;
  for(;;)
  {
    if(xSemaphoreTake(semaforoContador, portMAX_DELAY))// quando a interrupção conseguir pegar do semaforo ela trata
    {

      Serial.println("Tratando a ISR do botao");
      x = uxSemaphoreGetCount(semaforoContador);
      Serial.println("x: "+ String(x));
      delay(200);//para ver melhor o tratamento da interrupção
    }

  }
}
