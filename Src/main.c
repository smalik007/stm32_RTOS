/*
 * main.c
 *
 *  Created on: 10-Jan-2021
 *      Author: suhail
 */

#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

/* Defines */
#define TRUE 1
#define FALSE 0
#define AVAILABLE TRUE
#define NOT_AVAILABLE FALSE

/* Task handler to use to blocking, deleting, yeilding etc */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/* Task prototype */
void vTask1_handler(void* param);
void vTask2_handler(void* param);

// #define USE_SEMIHOSTING

#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

static void setupUSART3();
static void setupBoard();

/* Global variables */
uint8_t uart_accesskey = AVAILABLE;

void LOG_MSG(char* msg);

int main(void) {
  /* Reset the system clock to use HSI (internal clock at 64 Mhz) */
  /* HSI ON, PLL OFF, HSE OFF, System clock / cpu clock at 64 Mhz */
  HAL_RCC_DeInit();

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  // HAL_Init();

  /* Update the systemCoreClock variables */
  SystemCoreClockUpdate();

#ifdef USE_SEMIHOSTING
  /* Configration to enable semihosting for print  */
  initialise_monitor_handles();
  printf("Hello World\n");
#endif

  setupBoard();

  // printf("Hello start\n");

  LOG_MSG("Hello main");

  /* Create two task as follows, there setting the stack size to MINIMAL_SIZE = word * size of word in bytes */
  xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
  xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

  /* Start Schedular , No return from here*/
  vTaskStartScheduler();

  /* Control Never comes here */
  for (;;)
    ;
  return 0;
}

/* To setup the peripheral related configuration  */
static void setupBoard() { setupUSART3(); }

void vTask1_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    // printf("Hello from task -1\n");  
    LOG_MSG("Hello msg from Task-1");
    // if (uart_accesskey == AVAILABLE) {
    //   uart_accesskey = NOT_AVAILABLE;
    //   LOG_MSG("Hello From Task-1\n");
    //   uart_accesskey = AVAILABLE;

    //   /* Manually trigger context swithing, leaving the CPU and allowing other task to use the uart_accesskey*/
    //   taskYIELD();
    // }
  }
}

void vTask2_handler(void* param) {
  while (1) {
#ifdef USE_SEMIHOSTING
    printf("Hello From Task - 1\n");
#endif
    // if (uart_accesskey == AVAILABLE) {
    //   uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-2\n");
    //   uart_accesskey = AVAILABLE;
    //   taskYIELD();
    // }
  }
}

static void setupUSART3() {
  /*##-1- Configure the USART peripheral #####################################*/
  /* Put the USART peripheral in Synchronous Slave mode (USART Slave Mode) */
  COM_InitTypeDef* bsp_com1 = NULL;
  bsp_com1->BaudRate = 115200;
  bsp_com1->Parity = UART_PARITY_NONE;
  bsp_com1->StopBits = UART_STOPBITS_1;
  bsp_com1->WordLength = UART_WORDLENGTH_8B;
  bsp_com1->HwFlowCtl = UART_HWCONTROL_NONE;

  BSP_COM_Init(COM1, bsp_com1);
  BSP_COM_SelectLogPort(COM1);
  LOG_MSG("USART3\n");
}

void LOG_MSG(char* msg) {
  int DataIdx;
  int len = strlen(msg);

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    __io_putchar( msg[DataIdx] );
  }
}