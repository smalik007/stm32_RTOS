/*
 * boardSetup.h
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */
#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"
#include "global_var.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void CPU_CACHE_Enable(void);

void setupUSART3();
void setupBoard();
void setupLeds();
void LedOn(Led_TypeDef led);
void LedOff(Led_TypeDef led);
void LedToggle(Led_TypeDef led);
