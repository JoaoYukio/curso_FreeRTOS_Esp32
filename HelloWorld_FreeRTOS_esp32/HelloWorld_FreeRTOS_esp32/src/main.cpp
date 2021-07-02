#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//*******Exemplo criação de tasks FreeRTOS********//

#define LED 2//led built_in do esp32 

//Variavel de armazenamento do handle das tasks

TaskHandle_t task1handle = NULL;//inicializa o handle da task1 como NULL
TaskHandle_t task2handle = NULL;//inicializa o handle da task2 como NULL

//Protótipo das tasks

void vTask1 (void * pvParameters);
void vTask2 (void * pvParameters);



void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( LED , OUTPUT);

  //Inializamos as tarefas aqui para que o sistema inicie e a primeira coisa que ele faça é inicializar
  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &task1handle);
  xTaskCreate(vTask2, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &task2handle);

}

void loop() 
{
  vTaskDelay(1000);
}

void vTask1(void *pvParameters)//tarefa 1 faz um blink no led da placa
{
  for(;;)
  {
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void vTask2(void *pvParameters)// imprime na serial um contador de segundos
{
  int contador = 0; 
  for(;;)
  {
    Serial.println("Task 2:" + String(contador++));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

}


