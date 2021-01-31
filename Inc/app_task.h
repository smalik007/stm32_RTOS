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
#include "queue.h"
#include "timers.h"
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;
TaskHandle_t xTaskHandle3;
TaskHandle_t xTaskHandle4;

/* Queue Handles */
QueueHandle_t commad_queue;
QueueHandle_t uart_write_queue;


/* Exported constants --------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
typedef struct  AppCMD
{
    uint8_t cmd;
    uint8_t cmd_arg[10];
}App_cmd_t;

/* Exported functions ------------------------------------------------------- */
/* Task prototype ----------------------------------------------------------- */
void vTask1_menu_display(void* param);
void vTask2_cmd_handling(void* param);
void vTask3_cmd_processing(void* param);
void vTask4_uart_write(void* param);

void button_handler();

