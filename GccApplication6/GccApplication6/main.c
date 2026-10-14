#define F_CPU 16000000UL
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xSem1;
SemaphoreHandle_t xSem2;
SemaphoreHandle_t xSem3;

// Variable global para rastrear el paso exacto de las secuencias (0 al 11)
uint8_t paso_global = 0; 

// --- Configuración UART (igual que antes) ---
void UART_Init(void) {
    uint16_t ubrr = 103;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_SendString(const char *str) {
    while (*str) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *str++;
    }
}

// --- Función central de sincronización ---
// Esta función es llamada por las tareas cada vez que terminan de imprimir.
void ceder_paso(void) {
    paso_global++;
    
    // Si llegamos al final de la Secuencia C, reiniciamos a la Secuencia A
    if (paso_global >= 12) {
        paso_global = 0;
        UART_SendString("\r\n--- Reiniciando Ciclo ---\r\n");
    }

    /* 
     * Mapeo de los 12 pasos totales de las secuencias:
     * Secuencia A: 0(T1) - 1(T3) - 2(T2)
     * Secuencia B: 3(T2) - 4(T2) - 5(T3) - 6(T1)
     * Secuencia C: 7(T3) - 8(T3) - 9(T3) - 10(T1) - 11(T2)
     */
    switch(paso_global) {
        case 0: case 6: case 10:
            xSemaphoreGive(xSem1); // Le toca a la Tarea 1
            break;
        case 2: case 3: case 4: case 11:
            xSemaphoreGive(xSem2); // Le toca a la Tarea 2
            break;
        case 1: case 5: case 7: case 8: case 9:
            xSemaphoreGive(xSem3); // Le toca a la Tarea 3
            break;
    }
}

// --- Definición de las Tareas ---

void vTarea1(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(xSem1, portMAX_DELAY);
        UART_SendString("Tarea 1 - ");
        vTaskDelay(pdMS_TO_TICKS(400));
        
        ceder_paso(); // Pasa la pelota al árbitro central
    }
}

void vTarea2(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(xSem2, portMAX_DELAY);
        UART_SendString("Tarea 2 - ");
        vTaskDelay(pdMS_TO_TICKS(400));
        
        ceder_paso();
    }
}

void vTarea3(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(xSem3, portMAX_DELAY);
        UART_SendString("Tarea 3 - ");
        vTaskDelay(pdMS_TO_TICKS(400));
        
        ceder_paso();
    }
}
void vApplicationIdleHook(void) {
	// El procesador entra aquí cuando no hay tareas listas para ejecutar.
	// Para esta práctica, se deja vacía.
}

// --- Función Principal ---
int main(void) {
    UART_Init();

    xSem1 = xSemaphoreCreateBinary();
    xSem2 = xSemaphoreCreateBinary();
    xSem3 = xSemaphoreCreateBinary();

    // Arrancamos el ciclo liberando el semáforo 1 (paso_global inicia en 0)
    xSemaphoreGive(xSem1);

    xTaskCreate(vTarea1, "T1", 128, NULL, 1, NULL);
    xTaskCreate(vTarea2, "T2", 128, NULL, 1, NULL);
    xTaskCreate(vTarea3, "T3", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    for (;;) {}
    return 0;
}
