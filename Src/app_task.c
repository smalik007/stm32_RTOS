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

void vTask1_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-1\n");
      uart_accesskey = AVAILABLE;

#if USE_SEGGER_SYSVIEW == TRUE
      /* Prints for segger SYSview */
      SEGGER_SYSVIEW_Print("task1 - yielding");
      /* tells Segger that we are manually switching the context in case of co-operative scheduling */
      traceISR_EXIT_TO_SCHEDULER();
#endif

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

#if USE_SEGGER_SYSVIEW == TRUE
      SEGGER_SYSVIEW_Print("task2 - yielding");
      traceISR_EXIT_TO_SCHEDULER();
#endif
      taskYIELD();
    }
  }
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

void button_handler() { button_count = (button_count + 1) % 3; }
