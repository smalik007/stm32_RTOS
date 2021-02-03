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

char menu[] = {
    "\
\r\nLED_ON            ----> 1 \
\r\nLED_OFF           ----> 2 \
\r\nLED_TOGGLE        ----> 3 \
\r\nLED_TOGGLE_OFF    ----> 4 \
\r\nLED_READ_STATUS   ----> 5 \
\r\nRTC_DATETIME      ----> 6 \
\r\nEXIT_APP          ----> 0 \
\r\nType your option : "};

void vTask1_menu_display(void* param) {
  char* pData = menu;
  while (1) {
    xQueueSend(uart_write_queue, &pData, portMAX_DELAY);
    // wait until someone notifies
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
  }
}
void vTask2_cmd_handling(void* param) {
  char* usr_msg = (char*)malloc(200);
  while (1) {
    if (pdTRUE == xTaskNotifyWait(0, 0, NULL, portMAX_DELAY)) {
      /* Received the notification */
      sprintf(usr_msg, "Rx data: \r\n");
      LOG_MSG(usr_msg);
      LedToggle(LED_GREEN);
    }
  }
}

void vTask3_cmd_processing(void* param) {
  while (1) {
    /* code */
  }
}

void vTask4_uart_write(void* param) {
  char* pData = NULL;
  while (1) {
    xQueueReceive(uart_write_queue, &pData, portMAX_DELAY);
    LOG_MSG(pData);
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
