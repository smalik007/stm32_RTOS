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

static uint8_t getCommandCode(uint8_t* buffer);

void vTask1_menu_display(void* param) {
  char* pData = menu;
  while (1) {
    xQueueSend(uart_write_queue, &pData, portMAX_DELAY);
    // wait until someone notifies
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
  }
}
void vTask2_cmd_handling(void* param) {
  uint8_t command_code = 0;
  App_cmd_t* new_cmd;
  while (1) {
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    command_code = getCommandCode(usart_msg_buffer);
    new_cmd = (App_cmd_t*)pvPortMalloc(sizeof(App_cmd_t));
    new_cmd->cmd = command_code;
    // getArguments(new_cmd->cmd_arg);

    /* Send item to cmd queue */
    xQueueSend(commad_queue, &new_cmd, portMAX_DELAY);
  }
}

void vTask3_cmd_processing(void* param) {
  uint8_t command_code = 0;
  App_cmd_t* new_cmd;
  char task_msg[50];
  while (1) {
    xQueueReceive(commad_queue, (void*)&new_cmd, portMAX_DELAY);
    switch (new_cmd->cmd) {
      case CMD_LED_ON:
        LedOn(LED_GREEN);
        break;
      case CMD_LED_OFF:
        LedOff(LED_GREEN);
        break;
      case CMD_LED_TOGGLE:
        LedToggle(LED_GREEN);
        break;
      case CMD_LED_TOGGLE_OFF:
        LedOff(LED_GREEN);
        break;
      case CMD_LED_READ_STATUS:
        break;
      case CMD_RTC_DATETIME:
        break;
      default:
        break;
    }
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

uint8_t getCommandCode(uint8_t* buffer) {
  return buffer[buffer_rIdx] - 48;
  CIC_INC(buffer_rIdx, MAX_MSG_BUFF);
}