#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 2

SemaphoreHandle_t mutex;

TaskHandle_t vTask1Handle, vTask2Handle;

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);

 
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  int verificador;
  //criação do mutex e das tasks
  mutex = xSemaphoreCreateMutex();
  verificador = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE+1024, NULL, 3, &vTask1Handle);
  if(verificador == NULL){
    Serial.println("Task1 não pode ser criada, como eh essencial seu funcionamento entrarei num loop infinito!");
    for(;;);
  }
  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE+1024, NULL, 5, &vTask2Handle);
  if(verificador == NULL){
    Serial.println("Task2 não pode ser criada, como eh essencial seu funcionamento entrarei num loop infinito!");
    for(;;);
  }

}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(200));
}

void enviaInformação(char i)
{
  Serial.println("Enviando informação da task" + String(i));

}

void vTask1(void* pvParameters)
{

  for(;;)
  {
    if(xSemaphoreTake(mutex,portMAX_DELAY))//verifica se conseguiu pegar o mutex
    {
      enviaInformação(1);
      delay(1000);
      xSemaphoreGive(mutex);
      vTaskDelay(10);// da um tempo pra trocar as tasks, ou seja, um tempo com o mutex "livre" dependendo da prioridade algumas tasks nunca vao pgar o mutex se nao tiver isso
    }
  }
}

void vTask2(void* pvParameters)
{

  for(;;)
  {
    if(xSemaphoreTake(mutex, portMAX_DELAY))
    {
      enviaInformação(2);
      delay(500);
      xSemaphoreGive(mutex);
      vTaskDelay(10);// se nao deixar um tempo com a CPU liberada somente as tasks com prioridade grande vao funcionar
    }
  }
}