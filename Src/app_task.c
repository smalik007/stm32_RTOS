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

void vManagerTask(void* param) {
  unsigned int xWorkTicketId;
  portBASE_TYPE xStatus;
  char msg[100];

  /* Semaphore is created in the empty state, it must be first given before before it can be obtained */
  xSemaphoreGive(xWorkSem);

  while (1) {
    xWorkTicketId = (rand() & 0x1FF);

    xStatus = xQueueSend(xWorkQueue, &xWorkTicketId, portMAX_DELAY);
    if(xStatus != pdPASS) {
      sprintf(msg, "\r\nCouldn't Send to the queue\r\n");
      LOG_MSG(msg);
    } else {
      /* Manager task give the semaphore to be utilised by Employee task */
      xSemaphoreGive(xWorkSem);
      /* let the other task execute */
      taskYIELD();
    }
  }
}

void vEmployeeTask(void* param) {
  unsigned int ticket_id;
  portBASE_TYPE xStatus;
  char msg[100];
  while (1) {

    /* Tries to take Semaphore non-block state */
    xSemaphoreTake(xWorkSem, 0);

    /* Read with non-blocking state */
    xStatus = xQueueReceive(xWorkQueue, &ticket_id, 0);

    if(xStatus == pdPASS) {
      sprintf(msg, "Employee ID: %d\r\n", ticket_id);
      LOG_MSG(msg);
      vTaskDelay(ticket_id);
    } else {
      sprintf(msg, " Queue is Empty \r\n");
      LOG_MSG(msg);
    }

  }
}

void button_handler() {
  button_count = (button_count + 1) % 3;
  switch_priority = pdTRUE;
}

void rtos_delay(TickType_t delay_ms) {
  /* waste cpu cyles*/
  TickType_t ticks_to_wait = pdMS_TO_TICKS(delay_ms);

  TickType_t current_tick_count = xTaskGetTickCount();

  while (xTaskGetTickCount() < (current_tick_count + ticks_to_wait)) {
    ;
  }
}
