/*
 *  global.h
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#pragma once

#include <stdint.h>

/* Exported macro ------------------------------------------------------------*/

// #define USE_SEMIHOSTING
#define TRUE 1
#define FALSE 0
#define AVAILABLE TRUE
#define NOT_AVAILABLE FALSE
#define USE_SEGGER_SYSVIEW FALSE
#define USE_BUTTON_AS_INT TRUE

/* Global Variable ------------------------------------------------------------*/
typedef uint32_t timestamp_ms;
extern volatile timestamp_ms milliseconds;

extern void LOG_MSG(char* msg);
extern void button_handler();
