#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//*******Exemplo passagem de parâmetros para tasks FreeRTOS********//

#define LED2 2//led built_in do esp32 
#define LED14 14

//Variavel para passar para o contador//
int valor = 150;

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

  //Inializamos as tarefas aqui para que o sistema inicie e a primeira coisa que ele faça é inicializar
  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, (void*)LED2, 1, &task1handle);// usamos ponteiro pra void e apontamos para a variavel que vamos querer passar para task
  xTaskCreate(vTask2, "Task1", configMINIMAL_STACK_SIZE + 1024, (void*)valor, 2, &task2handle);// da mesma forma que o anterior, passamos o valor inicial de contagem para a task

}

void loop() 
{
  vTaskDelay(1000);
}

void vTask1(void *pvParameters)//tarefa 1 faz um blink no led da placa
{ 
  int pin = (int)pvParameters;// pega um valor dos parametros que foram passados, no caso qual led vai piscar
  pinMode(pin , OUTPUT);
  for(;;)
  {
    digitalWrite(pin, !digitalRead(pin));//inverte o sinal do led
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void vTask2(void *pvParameters)// imprime na serial um contador de segundos
{
  int valor = (int)pvParameters;// pega a variavel que foi passada
  int contador = valor;// podemos fazer apenas int contador =(int)pvParameters;
  for(;;)
  {
    Serial.println("Task 2:" + String(contador++));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

}


