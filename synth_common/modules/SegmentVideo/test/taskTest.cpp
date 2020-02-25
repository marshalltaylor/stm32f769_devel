/* Includes -- STD -----------------------------------------------------------*/
#include <stdio.h>

//FreeRTOS system
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

//FreeRTOS app
#include "taskLog.h"
#include "taskCommon.h"

#include "MidiClockDisplay.h"

extern MidiClockDisplay Segments;

//state inputs, not threadsafe
bool genTestLog = false;

char value1Str[8];

/* Declare a variable to hold the created event group. */
EventGroupHandle_t xTestEventGroup;

extern "C" void taskTestStart(void * argument)
{

	uint16_t stamp_ms = 0;
	uint8_t stamp_s = 0;
	bool testValueStateIdle = true;
	
	while(1)
	{
		// Wait for event group
		EventBits_t uxBits;
		
		//Pass on set flag
		uxBits = xEventGroupWaitBits(
            xTestEventGroup,
            TEST_EVENT_0, // logical OR macros
            pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
            pdFALSE,       /* Don't wait for both bits, either bit will do. */
            100 );/* Wait a maximum of 100ms for either bit to be set. */
			
		if( ( uxBits & TEST_EVENT_1 ) == TEST_EVENT_1 )
		{
			if( testValueStateIdle )
			{
				//rising edge, trigger one shot
				testValueStateIdle = false;

				sprintf( value1Str, "208" );
				Segments.showNewValue(value1Str);
				
				//Print through logger
				strMsg_t * msg = new strMsg_t();
				
				msg->id = 0;
				sprintf( msg->data, "Fade Triggered" );
				if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
				{
					//TODO: error on send
					delete msg;
				}
			}
		}
		else
		{
			//Clear
			testValueStateIdle = true;
		}
		if( ( uxBits & TEST_EVENT_0 ) == TEST_EVENT_0 )
		{
			// Event has triggered, run it
			stamp_ms += 50;
			if(stamp_ms > 999)
			{
				stamp_ms = 0;
				stamp_s++;
				if(stamp_s > 9)
				{
					stamp_s = 0;
				}
			}
			strMsg_t * msg = new strMsg_t();
			
			msg->id = 0;
			sprintf( msg->data, "Test: %02d:%03d", stamp_s, stamp_ms );
			if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
			{
				//TODO: error on send
				delete msg;
			}
			
			char buffer[5];
			sprintf( buffer, "%04d", stamp_ms );
			Segments.displayDrawClockNums(buffer);
			
			vTaskDelay( 50 );
		}
	}
}
