#include <Arduino.h>

//Bibliotecas FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 2//built_in led

TaskHandle_t xTaskADCHandle;

xSemaphoreHandle xSemaphore;

void vTaskADC(void* pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();

  if(xSemaphore == NULL)
  {
    Serial.println("Não foi possível criar o semaforo!");
    for(;;);
  }

  xTaskCreate(vTaskADC, "Leitor ADC", configMINIMAL_STACK_SIZE+1024, NULL, 2, &xTaskADCHandle);

}

void loop() 
{
  digitalWrite(LED, !digitalRead(LED));
  Serial.println("Valor do led: " + String(digitalRead(LED)));
  vTaskDelay(pdMS_TO_TICKS(500));
  xSemaphoreGive(xSemaphore);// acho que o semaforo só vair estar pronto pra uso quando uma task dar o primeiro semaforo
}

void vTaskADC(void* pvParameters)
{
  int valorADC = 0;
  for(;;)
  {
    if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){//checa se conseguiu pegar o semaforo, caso o valor dos ticks seja diferente do maximo ele não vai rodar se nao tiver o if
    valorADC = analogRead(34);
    Serial.println("Valor do ADC: " + String(valorADC));
    Serial.println(" ");
    }
  }

}
