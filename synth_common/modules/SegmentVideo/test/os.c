//FreeRTOS system
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
//#include "timers.h"

//FreeRTOS app
#include "taskLog.h"
#include "taskCommon.h"

//BSP
#include "bsp.h"

extern void xPortSysTickHandler(void);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
	//TODO: Should we check for scheduler running first?
	bspRegisterSysTickCallback(xPortSysTickHandler);

	// Example:
	//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
	//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );

	BaseType_t retVal;

	logQueue = xQueueCreate( 10, sizeof( strMsg_t* ) );

    /* Attempt to create the event group. */
    xTestEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( xTestEventGroup == NULL )
    {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
		while(1);
    }

	retVal = xTaskCreate( taskLogStart, "log", 1024, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskConsoleStart, "console", 512, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskTestStart, "test", 128, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

}

void osInit(void)
{

	MX_FREERTOS_Init();
	
	// Init other os objects
	
	
	// Start os and don't come back
	vTaskStartScheduler();
}
