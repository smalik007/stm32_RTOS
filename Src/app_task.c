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

void vTask1_handler(void* param) {
  UBaseType_t p1, p2;
  char* usr_msg = (char*)malloc(200);

  sprintf(usr_msg, "Task1 Running :\r\n");
  LOG_MSG(usr_msg);

  sprintf(usr_msg, "Task1 Priority : %ld\r\n", uxTaskPriorityGet(xTaskHandle1));
  LOG_MSG(usr_msg);

  sprintf(usr_msg, "Task2 Priority : %ld\r\n", uxTaskPriorityGet(xTaskHandle2));
  LOG_MSG(usr_msg);

  while (1) {
    if (switch_priority == pdTRUE) {
      switch_priority = pdFALSE;

      p1 = uxTaskPriorityGet(xTaskHandle1);
      p2 = uxTaskPriorityGet(xTaskHandle2);

      vTaskPrioritySet(xTaskHandle1, p2);
      vTaskPrioritySet(xTaskHandle2, p1);
    } else {
      LedToggle(LED_YELLOW);
      rtos_delay(1000);
    }
  }
  free(usr_msg);
}

void vTask2_handler(void* param) {
  UBaseType_t p1, p2;
  char* usr_msg = (char*)malloc(200);

  sprintf(usr_msg, "Task2 Running :\r\n");
  LOG_MSG(usr_msg);

  sprintf(usr_msg, "Task1 Priority : %ld\r\n", uxTaskPriorityGet(xTaskHandle1));
  LOG_MSG(usr_msg);

  sprintf(usr_msg, "Task2 Priority : %ld\r\n", uxTaskPriorityGet(xTaskHandle2));
  LOG_MSG(usr_msg);

  while (1) {
    if (switch_priority == pdTRUE) {
      switch_priority = pdFALSE;

      p1 = uxTaskPriorityGet(xTaskHandle1);
      p2 = uxTaskPriorityGet(xTaskHandle2);

      vTaskPrioritySet(xTaskHandle2, p1);
      vTaskPrioritySet(xTaskHandle1, p2);
    } else {
      LedToggle(LED_YELLOW);
      rtos_delay(200);
    }
  }
  free(usr_msg);
}

void vTask_handler_Led(void* param) {
  while (1) {
    if (button_count == 0) {
      LedToggle(LED_RED);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      LedToggle(LED_RED);
      vTaskDelay(200 / portTICK_PERIOD_MS);
    } else if (button_count == 1) {
      LedToggle(LED_YELLOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      LedToggle(LED_YELLOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    } else if (button_count == 2) {
      LedToggle(LED_GREEN);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      LedToggle(LED_GREEN);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

// void vTask_handler_Button(void* param) {
//   while (1) {
//     button_status = BSP_PB_GetState(BUTTON_USER);
//     if (button_status == BUTTON_PRESSED) {
//       button_count = (button_count + 1) % 3;
//     }
//     taskYIELD();
//   }
// }

void vTask_Led_notify_handler(void* param) {
  char* usr_msg = (char*)malloc(200);
  uint32_t notify_counter = 0;
  while (1) {
    if (pdTRUE == xTaskNotifyWait(0, 0, &notify_counter, portMAX_DELAY)) {
      /* Received the notification */
      sprintf(usr_msg, "Rx Notification : %ld\r\n", notify_counter);
      LOG_MSG(usr_msg);
      LedToggle(LED_GREEN);
    }
  }
  free(usr_msg);
}
void vTask_Button_notify_handler(void* param) {
  char* usr_msg = (char*)malloc(200);
  while (1) {
    if (BSP_PB_GetState(BUTTON_USER) == BUTTON_PRESSED) {
      vTaskDelay(200);
      sprintf(usr_msg, "Tx Notification \r\n");
      LOG_MSG(usr_msg);
      xTaskNotifyAndQuery(xTaskHandleLedNotify, 0, eIncrement, 0);
    }
  }
  free(usr_msg);
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
