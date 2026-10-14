/*
 * F_CPU debe estar definido en las propiedades del proyecto o aquí
 * para que el cálculo de los baudios sea correcto.
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

// --- Configuración Bare-Metal del UART para ATmega328P ---

void UART_Init(void) {
    // Cálculo del valor para el registro UBRR a 9600 baudios con 16MHz
    // UBRR = (F_CPU / (16 * Baud Rate)) - 1 = 103
    uint16_t ubrr = 103;
    
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    
    // Habilitar transmisión
    UCSR0B = (1 << TXEN0);
    
    // Formato de trama: 8 bits de datos, 1 bit de parada
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_SendString(const char *str) {
    while (*str) {
        // Esperar a que el buffer de transmisión esté vacío
        while (!(UCSR0A & (1 << UDRE0)));
        // Enviar el carácter
        UDR0 = *str++;
    }
}

// --- Definición de las Tareas de FreeRTOS ---

void vTarea1(void *pvParameters) {
    const char *mensaje = "Ejecutando Tarea 1\r\n";
    for (;;) {
        UART_SendString(mensaje);
        
        // Bloqueo temporal para no saturar la simulación de Proteus
        // y permitir observar el comportamiento.
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}

void vTarea2(void *pvParameters) {
    const char *mensaje = "Ejecutando Tarea 2\r\n";
    for (;;) {
        UART_SendString(mensaje);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTarea3(void *pvParameters) {
    const char *mensaje = "Ejecutando Tarea 3\r\n";
    for (;;) {
        UART_SendString(mensaje);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// --- Función Principal ---

int main(void) {
    // Inicializar el hardware (Puerto Serie)
    UART_Init();

    // Crear las 3 tareas solicitadas. 
    // Argumentos: Función, Nombre, Stack, Parámetros, Prioridad, Handler
    xTaskCreate(vTarea1, "T1", 128, NULL, 1, NULL);
    xTaskCreate(vTarea2, "T2", 128, NULL, 1, NULL);
    xTaskCreate(vTarea3, "T3", 128, NULL, 1, NULL);

    // Iniciar el planificador de FreeRTOS
    vTaskStartScheduler();

    // El programa nunca debería llegar aquí
    for (;;) {}
    return 0;
}
