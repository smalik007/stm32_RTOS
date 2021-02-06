/*
 * boardSetup.c
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#include "boardSetup.h"

#include <string.h>

#include "app_task.h"

/* Prototypes */
static void setupUSART3();
static void setupLeds();
static void setupButtons();

/* To setup the peripheral related configuration  */
void setupBoard() {
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Required when using segger system */
#if USE_SEGGER_SYSVIEW == TRUE
  DWT->CTRL |= (1 << 0);
#endif

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
  //  HAL_InitTick();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  setupUSART3();
  setupLeds();
  setupButtons();
}

void setupUSART3() {
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
}

void setupLeds() {
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_YELLOW);
  BSP_LED_Init(LED_RED);
}

void LedOn(Led_TypeDef led) { BSP_LED_On(led); }
void LedOff(Led_TypeDef led) { BSP_LED_Off(led); }
void LedToggle(Led_TypeDef led) { BSP_LED_Toggle(led); }

void setupButtons() {
#if USE_BUTTON_AS_INT == TRUE
  /* EXTI interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
#else
  /* GPIO MODE */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
#endif
}

void LOG_MSG(char* msg) {
  int DataIdx;
  int len = strlen(msg);

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    __io_putchar(msg[DataIdx]);
  }
}

/**
 * @brief  This function handles EXTI0_IRQ Handler.
 * @param  None
 * @retval None
 */
void EXTI15_10_IRQHandler(void) {
  /* Following function clears IT pending bits and in returns call the below BSP_PB_Callback*/
  // BSP_PB_IRQHandler(BUTTON_USER);
  button_handler();
}

/* Interrupt callback function for user button */
void BSP_PB_Callback(Button_TypeDef Button) {
  if (Button == BUTTON_USER) {
    button_handler();
  }
}

/* Uart RxISR callback function */
void BSP_COM_Rx_Callback(COM_TypeDef COM) {
  BaseType_t xHigherPriorityTask = pdFALSE;

  if (__HAL_USART_GET_FLAG(&hcom_uart[COM], USART_FLAG_RXNE)) {
    uint8_t data_bytes = 0;
    HAL_UART_Receive(&hcom_uart[COM], (uint8_t*)&data_bytes, sizeof(data_bytes), 0);
    // HAL_UART_Receive_IT(&hcom_uart[COM], (uint8_t*)&data_bytes, 1);
    // HAL_UART_IRQHandler(&hcom_uart[COM]);

    if (COM == COM1) {
      usart_msg_buffer[buffer_wIdx] = data_bytes;
      CIC_INC(buffer_wIdx, MAX_MSG_BUFF);

      /* User has finished entering data */
      // Notify the cmd_handling task
      // xTaskNotifyFromISR(xTaskHandle2, 0, eNoAction, &xHigherPriorityTask);
      // xTaskNotifyFromISR(xTaskHandle1, 0, eNoAction, &xHigherPriorityTask);
    }
    __HAL_USART_ENABLE_IT(&hcom_uart[COM], USART_IT_RXNE);
    // __HAL_USART_CLEAR_FLAG(&hcom_uart[COM], USART_CLEAR_TCF);
  }

  if (xHigherPriorityTask == pdTRUE) {
    taskYIELD();
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
void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
  }

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
  if (ret != HAL_OK) {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if (ret != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
  LedOn(LED_RED);

  while (1) {
  }
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
void CPU_CACHE_Enable(void) {
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/* override function to enable RX interrupt etc, this is application specific confihuration
 the function will be called from the HAL Layer  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
  __HAL_USART_ENABLE_IT(huart, USART_IT_RXNE);
  HAL_NVIC_SetPriority(COM1_IRQn, BSP_COM1_IT_PRIORITY, 1);
  HAL_NVIC_EnableIRQ(COM1_IRQn);
}

void USART3_IRQHandler(void) {
  //  BSP_COM_IRQHandler(COM1);
  BSP_COM_Rx_Callback(COM1);
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef* UartHandle) {
//   /* Set transmission flag: transfer complete */
//   //  UartReady = SET;

//   uint8_t data_byte = 0;
//   uint8_t buffer_rdIdx_local = 0;
//   data_byte = usart_msg_buffer[buffer_rdIdx_local];
//   while (data_byte != NULL || buffer_rdIdx_local < sizeof(usart_msg_buffer)) {
//     if (data_byte == '\r') {
//       xTaskNotifyFromISR(xTaskHandle2, 0, eNoAction, 0);
//       break;
//     }
//   }
//   __HAL_USART_ENABLE_IT(UartHandle, USART_IT_RXNE);
// }
