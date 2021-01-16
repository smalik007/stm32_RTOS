/*
 *  main.c
 *
 *  Created on: 10-Jan-2021
 *      Author: suhail
 */

#include "main.h"

int main(void) {
  setupBoard();

#ifdef USE_SEMIHOSTING
  /* Configration to enable semihosting for print  */
  initialise_monitor_handles();
  printf("Hello World\r\n");
#endif

  LOG_MSG("Hello main");
  LedOn(LED_GREEN);
//  LedOn(LED_YELLOW);
//  LedOn(LED_RED);

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
