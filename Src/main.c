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

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

int main(void) {

 /* Enable the CPU Cache */
  CPU_CACHE_Enable();

   /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

   /* Configure the system clock to 400 MHz */
  SystemClock_Config();


  /* Reset the system clock to use HSI (internal clock at 64 Mhz) */
  /* HSI ON, PLL OFF, HSE OFF, System clock / cpu clock at 64 Mhz */
  // HAL_RCC_DeInit();

  /* Update the systemCoreClock variables */
  // SystemCoreClockUpdate();


#ifdef USE_SEMIHOSTING
  /* Configration to enable semihosting for print  */
  initialise_monitor_handles();
  printf("Hello World\r\n");
#endif

  setupBoard();

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
    LOG_MSG("Hello msg from Task-1\n");
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
  uint32_t millis = HAL_GetTick();
  LOG_MSG(millis);
//  LOG_MSG("USART3\n");
}

void LOG_MSG(char* msg) {
  int DataIdx;
  int len = strlen(msg);

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    __io_putchar( msg[DataIdx] );
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  // /* Turn LED3 on */
  // BSP_LED_On(LED3);

  // while(1)
  // {
  // }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
