/*
 *  app_task.h
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#pragma once

#include "FreeRTOS.h"
#include "boardSetup.h"
#include "global_var.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xPeriodic;
TaskHandle_t xHandler;

/* Queue Handles */
/* QueueHanle_t is typedef of xQueueHandle only */
QueueHandle_t xWorkQueue;

/* Semaphore Handle */
xSemaphoreHandle xCountingSem;

/* Exported constants --------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
typedef struct AppCMD {
  uint8_t cmd;
  uint8_t cmd_arg[10];
} App_cmd_t;

/* Exported functions ------------------------------------------------------- */
/* Task prototype ----------------------------------------------------------- */
void vPeriodic(void* param);
void vHandler(void* param);

void button_handler();

typedef enum COMMAND_ENUM {
  CMD_EXIT_APP = 0,
  CMD_LED_ON = 1,
  CMD_LED_OFF = 2,
  CMD_LED_TOGGLE = 3,
  CMD_LED_TOGGLE_OFF = 4,
  CMD_LED_READ_STATUS = 5,
  CMD_RTC_DATETIME = 6,
  CMD_MAX_CMD = 7

} COMMAND_ENUM_E;
