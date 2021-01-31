/*
 *  main.c
 *
 *  Created on: 10-Jan-2021
 *      Author: suhail
 */

#include "main.h"

static void init_vars(void);

int main(void) {

  init_vars();
  setupBoard();

#ifdef USE_SEMIHOSTING
  /* Configration to enable semihosting for print  */
  initialise_monitor_handles();
  printf("Hello World\r\n");
#endif

  LOG_MSG("Hello main");
  // LedOn(LED_GREEN);
  //  LedOn(LED_YELLOW);
  //  LedOn(LED_RED);

#if USE_SEGGER_SYSVIEW == TRUE
  /* Start Recording Segger */
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();
#endif

  /* create queue  */
  commad_queue = xQueueCreate(10, sizeof(App_cmd_t*));
  uart_write_queue = xQueueCreate(10, sizeof(char*));

  if(commad_queue != NULL) {
  xTaskCreate(vTask1_menu_display, "Menu-Display", 500, NULL, 2, &xTaskHandle1);
  xTaskCreate(vTask2_cmd_handling, "Menu-Display", 500, NULL, 2, &xTaskHandle2);
  xTaskCreate(vTask3_cmd_processing, "Menu-Display", 500, NULL, 2, &xTaskHandle3);
  xTaskCreate(vTask4_uart_write, "Menu-Display", 500, NULL, 2, &xTaskHandle4);

  /* Start Schedular , No return from here*/
  vTaskStartScheduler();
  } else {
    LOG_MSG("QUEUE creation failed");
  }

  /* Control Never comes here */
  for (;;)
    ;
  return 0;
}


void init_vars(void) {
  usart3_buffer_rIdx = 0;
  usart3_buffer_wIdx = 0;
}
