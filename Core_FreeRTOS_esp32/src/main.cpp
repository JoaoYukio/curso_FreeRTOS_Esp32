#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//*******Exemplo criar tarefas em núcleos diferentes FreeRTOS********//
//*******Além disso vemos como fazemos para ver quanto de mémoria usamos para cada task dependendo do que foi especifado para ela*******/////

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
  xTaskCreatePinnedToCore(vTask1, "Task1", configMINIMAL_STACK_SIZE, (void*)LED2, 1,&task1handle, APP_CPU_NUM);// a task blink ta no core 1
  xTaskCreatePinnedToCore(vTask2, "Task2", configMINIMAL_STACK_SIZE+1024, (void*)valor, 1,&task2handle, PRO_CPU_NUM);// a task serial ta no core 0

}

void loop() 
{
  vTaskDelay(1000);
}

void vTask1(void *pvParameters)//tarefa 1 faz um blink no led da placa
{ 
  UBaseType_t uxHighWaterMark;
  int pin = (int)pvParameters;// pega um valor dos parametros que foram passados, no caso qual led vai piscar
  pinMode(pin , OUTPUT);
  for(;;)
  {
    digitalWrite(pin, !digitalRead(pin));//inverte o sinal do led
    vTaskDelay(pdMS_TO_TICKS(300));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("Quantidade usado de memória para task1: ");
    Serial.println(uxHighWaterMark);

  }
}

void vTask2(void *pvParameters)// imprime na serial um contador de segundos
{
  UBaseType_t uxHighWaterMark;
  int valor = (int)pvParameters;// pega a variavel que foi passada
  int contador = valor;// podemos fazer apenas int contador =(int)pvParameters;
  for(;;)
  {
    Serial.println("Task 2:" + String(contador++));
    vTaskDelay(pdMS_TO_TICKS(1000));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("Quantidade usado de memória para task2: ");//pcTaskGetTaskName(NULL) pega o nome da task
    Serial.println(uxHighWaterMark);
  }

}


