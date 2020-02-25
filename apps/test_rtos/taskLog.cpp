//FreeRTOS system
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//FreeRTOS app
#include "taskLog.h"

//Arduino
#include "Arduino.h"
#include "interface.h"

//BSP
//#include "main_cubemx.h"

#define Serial Serial6

QueueHandle_t logQueue = NULL;

extern "C" void taskLogStart(void * argument)
{
	while(1)
	{
		strMsg_t * msg = NULL;
		while(xQueueReceive( logQueue, &msg, 0 ) == pdPASS)
		{
			//print output
			switch( msg->id )
			{
				case -1:
				{
					Serial6.println("BANG!");
					break;
				}
				case 0:
				{
					Serial6.print(msg->data);
					break;
				}
				default:
				{
					Serial6.print("[");
					Serial6.print(msg->id);
					Serial6.print("] ");
					Serial6.print(msg->data);
					break;
				}
			}

			delete msg;
		}
		// Don't smash it!
		vTaskDelay( 50 );
	}
}
