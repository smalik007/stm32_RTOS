/*
 *  app_task.h
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#pragma once

#include "FreeRTOS.h"
#include "global.h"
#include "task.h"
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/* Exported constants --------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
static uint8_t uart_accesskey = AVAILABLE;

/* Exported functions ------------------------------------------------------- */
/* Task prototype ----------------------------------------------------------- */
void vTask1_handler(void* param);
void vTask2_handler(void* param);
