/*
 *  app_task.c
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#include "app_task.h"

void vTask1_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-1\n");
      uart_accesskey = AVAILABLE;

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
      taskYIELD();
    }
  }
}
