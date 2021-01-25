/*
 * FreeRTOSConfig.c
 *
 *  Created on: 26-Jan-2021
 *      Author: suhail
 */

#include "FreeRTOSConfig.h"
#include "global_var.h"
#include "stm32h7xx_hal.h"

volatile timestamp_ms milliseconds = 0;

void vApplicationIdleHook(void) {
  /* Send CPU to normal sleep mode untill interrupt (systick or any other) */
  __WFI();
}

void vApplicationTickHook(void) {
  /* Ideally should be milliseconds += (1 / configTICK_RATE_HZ) * 1000; */
  milliseconds++;
}

void vApplicationStackOverflowHook(void) {
    LOG_MSG("Stack Overflow !!!");
}

void vApplicationMallocFailedHook(void) {
    LOG_MSG("Mem Alloc Err !!!");
}