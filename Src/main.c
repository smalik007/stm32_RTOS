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
  // xWorkQueue = xQueueCreate(10, sizeof(unsigned int));
  xCountingSem =  xSemaphoreCreateCounting(10, 0);

  if (xCountingSem != NULL) {
    xTaskCreate(vPeriodic, "Task-Periodic", 500, NULL, 3, &xPeriodic);
    xTaskCreate(vHandler, "Task-Handler", 500, NULL, 2, &xHandler);

    /* Start Schedular , No return from here*/
    vTaskStartScheduler();
  } else {
    LOG_MSG("QUEUE/Semaphore creation failed");
  }

  /* Control Never comes here */
  for (;;)
    ;
  return 0;
}

void init_vars(void) {
  buffer_rIdx = 0;
  buffer_wIdx = 0;
}
