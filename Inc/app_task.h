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
#include "task.h"
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

TaskHandle_t xTaskHandleLed;
TaskHandle_t xTaskHandleButton;

/* Exported constants --------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* Task prototype ----------------------------------------------------------- */
void vTask1_handler(void* param);
void vTask2_handler(void* param);

void vTask_handler_Led(void* param);
void vTask_handler_Button(void* param);
