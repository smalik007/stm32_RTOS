/*
 *  app_task.c
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#include "app_task.h"

uint8_t uart_accesskey = AVAILABLE;

void vTask1_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-1\n");
      uart_accesskey = AVAILABLE;

      /* Prints for segger SYSview */
      SEGGER_SYSVIEW_Print("task1 - yielding");

      /* tells Segger that we are manually switching the context in case of co-operative scheduling */
      traceISR_EXIT_TO_SCHEDULER();

      /* Manually trigger context swithing, leaving the CPU and allowing other task to use the uart_accesskey*/
      taskYIELD();
    }
  }
}

void vTask2_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-2\n");
      uart_accesskey = AVAILABLE;

      SEGGER_SYSVIEW_Print("task2 - yielding");

      traceISR_EXIT_TO_SCHEDULER();

      taskYIELD();
    }
  }
}
