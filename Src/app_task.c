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

void vTask1_menu_display(void* param) {
  while (1)
  {
    /* code */
  }
  
}
void vTask2_cmd_handling(void* param) {
  char* usr_msg = (char*)malloc(200);
  while (1)
  {
    if (pdTRUE == xTaskNotifyWait(0, 0, NULL, portMAX_DELAY)) {
      /* Received the notification */
      sprintf(usr_msg, "Rx data: \r\n");
      LOG_MSG(usr_msg);
      LedToggle(LED_GREEN);
    }
  }
  
}

void vTask3_cmd_processing(void* param) {
  while (1)
  {
    /* code */
  }
  
}

void vTask4_uart_write(void* param) {
  while (1)
  {
    /* code */
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
