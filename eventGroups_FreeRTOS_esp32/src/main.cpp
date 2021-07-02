#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#define LED 2
#define TASK1_FLAG (1<<0)
#define TASK2_FLAG (1<<1) 

int tempo =0;
TaskHandle_t xTask1Handle, xTask2Handle;
TimerHandle_t xTimer;

EventGroupHandle_t xEventos;

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);

void callbackTimer1(TimerHandle_t pxTimer);


void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);

  xEventos = xEventGroupCreate();
  xTimer = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, 0, callbackTimer1);

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask2Handle); 

  xTimerStart(xTimer,0); 
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(500));
}

void vTask1(void* pvParameters)
{
  EventBits_t xBits;
  for(;;)
  {
    xBits = xEventGroupWaitBits(xEventos, TASK1_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);// Espera o bit da task1, poderia ser 0x0 tambem
    Serial.println("Task1 saiu do estado de bloqueio!");
  }
}
void vTask2(void* pvParameters)
{
  EventBits_t xBits;
  for(;;)
  {
    xBits = xEventGroupWaitBits(xEventos, TASK2_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task2 saiu do estado de bloqueio!");
  }
}
void callBackTimer1(TimerHandle_t pxTimer)
{
  tempo++;
  if(tempo == 5)
  {
    xEventGroupSetBits(xEventos, TASK1_FLAG);
  }else if(tempo == 10)
  {
    xEventGroupSetBits(xEventos, TASK2_FLAG);
    tempo =0;
  }
}
