/*
 * main.c
 *
 *  Created on: 10-Jan-2021
 *      Author: suhail
 */

#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/* Task prototype */
void vTask1_handler(void* param);
void vTask2_handler(void* param);

// void LOG_MSG(char* msg);

int main(void) {
  /* Reset the system clock to use HSI (internal clock at 16 MHz) */
  /* HSI ON, PLL OFF, HSE OFF, System clock / cpu clock */
  HAL_RCC_DeInit();

  /* Update the systemCoreClock variables */
  SystemCoreClockUpdate();

  /* Create two task as follows, there setting the stack size to MINIMAL_SIZE = word * size of word in bytes */
  xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
  xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

  /* Start Schedular , No return from here*/
  vTaskStartScheduler();

  /* Control Never comes here */
  for (;;)
    ;
  return 0;
}

void vTask1_handler(void* param) {
  while (1) {
    // if (uart_accesskey == AVAILABLE) {
    //   uart_accesskey = NOT_AVAILABLE;
    //   LOG_MSG("Hello From Task-1\n");
    //   uart_accesskey = AVAILABLE;

    //   /* Manually trigger context swithing, leaving the CPU and allowing other task to use the uart_accesskey*/
    //   taskYIELD();
    // }
  }
}

void vTask2_handler(void* param) {
  while (1) {
    // if (uart_accesskey == AVAILABLE) {
    //   uart_accesskey = NOT_AVAILABLE;
    //   LOG_MSG("Hello From Task-2\n");
    //   uart_accesskey = AVAILABLE;
    //   taskYIELD();
    // }
  }
}

// void LOG_MSG(char* msg) {
//   for (uint32_t i = 0; i < strlen(msg); i++) {
//     /* You first need to check whether the TX register is empty, You can check this by checking the status of USART_FLAG_TXE register is set(if empty) */
//     while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
//       ;
//     USART_SendData(USART2, msg[i]);
//   }
// }
