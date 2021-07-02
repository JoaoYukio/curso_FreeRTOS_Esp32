//********Exemplo para criar queues, adicionar e ler dados dela*************//

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LED 2

QueueHandle_t xFila;
TaskHandle_t xTask1Handle, xTask2Handle;

void vTask1(void* pvParameters);

void vTask2(void* pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  BaseType_t xReturned;

  xFila = xQueueCreate(5, sizeof(int));//cria a fila 
  if(xFila==NULL)//verifica se a fila foi criada
  {
    Serial.println("Não foi possível criar a fila!");
    for(;;);
  }
  xReturned  =  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024,NULL, 1, &xTask1Handle);//nao esquecer que precisa passar o ENDEREÇO do handle
  if(xReturned == pdFAIL)//testa a inicialização da task
  {
    Serial.println("Não foi possível criar a task1!");
    for(;;);

  }
  xReturned  =  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE + 1024,NULL, 1, &xTask2Handle);//nao esquecer que precisa passar o ENDEREÇO do handle
  if(xReturned == pdFAIL)//testa a inicialização da task
  {
    Serial.println("Não foi possível criar a task2!");
    for(;;);

  }

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, !digitalRead(LED));//Lembrar que o setup e o void são TASKS.
  vTaskDelay(500);
}

void vTask1(void* pvParameters)
{
  int cont = 0;//tem que ser do tipo int pq a fila usa int

  for(;;)
  {
    if(cont<10)
    {
      xQueueSend(xFila, &cont, portMAX_DELAY);//novamente aqui se usa o endereço do valor que vamos colocar na fila!
      cont++;
    }else
    {
      cont=0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void vTask2(void* pvParameters)
{
  int valorRecebido = 0;
  for(;;)
  {
    if(xQueueReceive(xFila, &valorRecebido, pdMS_TO_TICKS(1000))==pdTRUE)
    {
      Serial.println("Valor recebido: "+ String(valorRecebido) );
    }else
    {
      Serial.println("TimeOut");
    }
  
  }

}
