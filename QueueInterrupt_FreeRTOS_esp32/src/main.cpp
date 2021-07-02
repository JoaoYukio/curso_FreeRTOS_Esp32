//*******Exemplo de queue com interrupções**********//
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LED 2
#define BOT 12

QueueHandle_t xFila;
TaskHandle_t xTask1handle;

void vTask1(void*pvParameters);//os nomes iniciam com x e v dependendo do que eles representam, ler nas referencias do FreeRTOS o que quer dizer

void trataISR_BOT()
{
  static int valor;
  valor++;
  xQueueSendFromISR(xFila, &valor, NULL);//usamos o ISR já que estamos dentro de uma interrupção!

}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BOT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOT), trataISR_BOT, FALLING);

  xFila = xQueueCreate(5, sizeof(int));

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1handle);

}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void*pvParameters)
{
  int valorRecebido;

  for(;;)
  {
    xQueueReceive(xFila, &valorRecebido, portMAX_DELAY); 
    Serial.println("Botão pressionado " + String(valorRecebido) + " vezes");


  }

}

