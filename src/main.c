/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "task.h"

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

#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

static void setupUSART2();
static void setupBoard();

/* Global variables */
uint8_t uart_accesskey = AVAILABLE;

void LOG_MSG(char* msg);

int main(void) {
  /* Reset the system clock to use HSI (internal clock at 16 MHz) */
  /* HSI ON, PLL OFF, HSE OFF, System clock = 16MHz, cpu clock = 16MHz */
  RCC_DeInit();

  /* Update the systemCoreClock variables */
  SystemCoreClockUpdate();

#ifdef USE_SEMIHOSTING
  /* Configration to enable semihosting for print  */
  initialise_monitor_handles();
  printf("Hello World\n");
#endif

  setupBoard();

  char usr_msg[250];

  LOG_MSG("Hello World\n");

  /* Create two task as follows, there setting the stack size to MINIMAL_SIZE = word * size of word in bytes */
  xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
  xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

  vTaskStartScheduler();

  for (;;)
    ;
}

void vTask1_handler(void* param) {
  while (1) {
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-1\n");
      uart_accesskey = AVAILABLE;

      /* Manually trigger context swithing, leaving the CPU and allowing other task to use the uart_accesskey*/
      taskYIELD();
    }
  }
}

void vTask2_handler(void* param) {
  while (1) {
    if (uart_accesskey == AVAILABLE) {
      uart_accesskey = NOT_AVAILABLE;
      LOG_MSG("Hello From Task-2\n");
      uart_accesskey = AVAILABLE;
      taskYIELD();
    }
  }
}

static void setupUSART2() {
  GPIO_InitTypeDef gpioPinConfig;
  USART_InitTypeDef usart2Config;

  /* 1. Enable UART clock & GPIOA*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Pin Config: PA2 -> Tx , PA3 - Rx */
  memset(&gpioPinConfig, 0, sizeof(gpioPinConfig));
  gpioPinConfig.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  gpioPinConfig.GPIO_Mode = GPIO_Mode_AF;
  gpioPinConfig.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &gpioPinConfig);

  /* Set the GPIO in alternate functionality mode */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // PA2 alternate function set to Tx
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // PA3 alternate function set to Rx

  /* Uart Peripheral Config */

  memset(&usart2Config, 0, sizeof(usart2Config));
  usart2Config.USART_BaudRate = 115200;
  usart2Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart2Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart2Config.USART_Parity = USART_Parity_No;
  usart2Config.USART_StopBits = USART_StopBits_1;
  usart2Config.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &usart2Config);
  USART_Cmd(USART2, ENABLE);
}

/* To setup the peripheral related configuration  */
static void setupBoard() { setupUSART2(); }

void LOG_MSG(char* msg) {
  for (uint32_t i = 0; i < strlen(msg); i++) {
    /* You first need to check whether the TX register is empty, You can check this by checking the status of USART_FLAG_TXE register is set(if empty) */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
      ;
    USART_SendData(USART2, msg[i]);
  }
}
