/*
 *  global.h
 *
 *  Created on: 16-Jan-2021
 *      Author: suhail
 */

#pragma once

/* Exported macro ------------------------------------------------------------*/

// #define USE_SEMIHOSTING
#define TRUE 1
#define FALSE 0
#define AVAILABLE TRUE
#define NOT_AVAILABLE FALSE

/* Global Variable ------------------------------------------------------------*/
extern uint32_t milliseconds;

extern void LOG_MSG(char* msg);
