#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Configuración del Kernel para ATmega328P */
#define configUSE_PREEMPTION            1
#define configUSE_TIME_SLICING          1
#define configCPU_CLOCK_HZ ( ( unsigned long ) 16000000 )
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES            ( 4 )
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 85 ) /* En palabras (170 bytes) */
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 1500 ) )   /* 1.5 KB de los 2 KB disponibles */
#define configMAX_TASK_NAME_LEN         ( 8 )
#define configUSE_TRACE_FACILITY        0
#define configUSE_16_BIT_TICKS          1 /* Obligatorio en 1 para arquitecturas de 8 bits (AVR) */
#define configIDLE_SHOULD_YIELD         1
#define configUSE_MUTEXES               1 /* Necesario para los Ejercicios 3 y 4 */
#define configQUEUE_REGISTRY_SIZE       0
#define configCHECK_FOR_STACK_OVERFLOW  0
#define configUSE_RECURSIVE_MUTEXES     0
#define configUSE_MALLOC_FAILED_HOOK    0
#define configUSE_APPLICATION_TASK_TAG  0
#define configUSE_COUNTING_SEMAPHORES   1
#define configGENERATE_RUN_TIME_STATS   0
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
/* Rutinas cooperativas (Deshabilitadas) */
#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Funciones de la API a incluir (1 = Incluir, 0 = Excluir) */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             0
#define INCLUDE_vTaskCleanUpResources   0
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelay              1 /* Obligatorio para generar bloqueos temporales */

#endif /* FREERTOS_CONFIG_H */