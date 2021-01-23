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
  // LedOn(LED_GREEN);
  //  LedOn(LED_YELLOW);
  //  LedOn(LED_RED);

#if USE_SEGGER_SYSVIEW == TRUE
  /* Start Recording Segger */
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();
#endif

  /* Create two task as follows, there setting the stack size to MINIMAL_SIZE = word * size of word in bytes */
  xTaskCreate(vTask1_handler, "Task-1", 500, NULL, 2, &xTaskHandle1);
  xTaskCreate(vTask2_handler, "Task-2", 500, NULL, 3, &xTaskHandle2);

  // xTaskCreate(vTask_handler_Led, "Led-Task", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandleLed);
  // xTaskCreate(vTask_handler_Button, "Button-Task", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandleButton);

  // xTaskCreate(vTask_Led_notify_handler, "Led-Notfied", 500, NULL, 2, &xTaskHandleLedNotify);
  // xTaskCreate(vTask_Button_notify_handler, "Button-Notify", 500, NULL, 2, &xTaskHandleButtonNotify);

  /* Start Schedular , No return from here*/
  vTaskStartScheduler();

  /* Control Never comes here */
  for (;;)
    ;
  return 0;
}
