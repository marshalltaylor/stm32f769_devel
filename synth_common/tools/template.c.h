/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//BSP usage only !!!
///* Includes -- HAL -----------------------------------------------------------*/
//#include "main_cubemx.h"
//#include "tim.h"
//#include "stm32f4xx_it.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"
#include "taskCommon.h"

/* Includes -- modules -------------------------------------------------------*/
#include "uCModules.h"
#include "logging.h"
#include "MidiClockDisplay.h"

/* References ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/






#include "MidiClockDisplay.h"


#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf dummy.printf
Logging dummy;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif
#ifdef USE_LOGGING
	dummy.setStamp("dummy", 5);
	//dummy.setMode(LOG_MODE_AUTO);
#endif