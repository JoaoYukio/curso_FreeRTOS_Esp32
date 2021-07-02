#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>

#define LED1 2
#define LED2 14
#define BT   12

TaskHandle_t xTask1;
TimerHandle_t xTimer1, xTimer2; 

void vTask1(void* pvParameters);

void callBackTimer1(TimerHandle_t xTimer);
void callBackTimer2(TimerHandle_t xTimer);


void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  xTimer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer1);//autoreload == pdTRUE
  xTimer2 = xTimerCreate("Timer2", pdMS_TO_TICKS(10000), pdFALSE, 0, callBackTimer2);//oneshot == pdFalse

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE +1024, NULL, 1, &xTask1);

  xTimerStart(xTimer1, 0); // inicia imediatamente o timer 1
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void* pvParameters)
{
  uint8_t debouncingTimer = 0;
  for(;;)
  {
    if((digitalRead(BT) == LOW) && (xTimerIsTimerActive(xTimer2)==pdFALSE))
    {
      debouncingTimer++;
        if(debouncingTimer >= 10)
        {
          debouncingTimer = 0;
          digitalWrite(LED2, HIGH);
          xTimerStart(xTimer2,0);
          Serial.println("Iniciando o timer2");
        }else
        {
          debouncingTimer = 0;
        }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void callBackTimer1(TimerHandle_t xTimer)
{
  digitalWrite(LED1, !digitalRead(LED1));
}
void callBackTimer2(TimerHandle_t xTimer)
{
  digitalWrite(LED2, LOW);
}


