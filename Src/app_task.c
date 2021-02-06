/*
 *  app_task.c
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#include "app_task.h"

#include <stdio.h>

uint8_t uart_accesskey = AVAILABLE;
uint8_t button_status = BUTTON_RELEASED;
uint8_t button_count = 0;

uint8_t switch_priority = pdFALSE;


void vPeriodic(void* param) {
  char msg[100];

  while (1) {
   vTaskDelay(pdMS_TO_TICKS(500));
   sprintf(msg, "\r\n Periodic Task - Pends Button ISR\r\n");
   LOG_MSG(msg);

   HAL_NVIC_SetPendingIRQ(EXTI15_10_IRQn);

   sprintf(msg, "\r\n Periodic Task - Resumes and waits..\r\n");
   LOG_MSG(msg);
   }
}

void vHandler(void* param) {
  char msg[100];
  while (1) {
    /* Decrements xCountingSem value */
    xSemaphoreTake(xCountingSem, portMAX_DELAY);

    sprintf(msg, "\r\nHandler Processing...\r\n");
    LOG_MSG(msg);
  }
}

void button_handler() {
  button_count = (button_count + 1) % 3;
  switch_priority = pdTRUE;
  portBASE_TYPE xHigherPTaskWake = pdFALSE;

  char msg[50];
  sprintf(msg, "\r\n==>Button ISR\r\n");
  LOG_MSG(msg);

  /* Give semaphore multiple times to latch multiple interrupts */
  xSemaphoreGiveFromISR(xCountingSem, &xHigherPTaskWake);
  xSemaphoreGiveFromISR(xCountingSem, &xHigherPTaskWake);
  xSemaphoreGiveFromISR(xCountingSem, &xHigherPTaskWake);
  xSemaphoreGiveFromISR(xCountingSem, &xHigherPTaskWake);
  xSemaphoreGiveFromISR(xCountingSem, &xHigherPTaskWake);

  portEND_SWITCHING_ISR(xHigherPTaskWake);
}

void rtos_delay(TickType_t delay_ms) {
  /* waste cpu cyles*/
  TickType_t ticks_to_wait = pdMS_TO_TICKS(delay_ms);

  TickType_t current_tick_count = xTaskGetTickCount();

  while (xTaskGetTickCount() < (current_tick_count + ticks_to_wait)) {
    ;
  }
}
