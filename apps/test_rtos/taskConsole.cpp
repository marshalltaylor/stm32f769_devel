//FreeRTOS system
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

//FreeRTOS app
#include "taskLog.h"
#include "taskCommon.h"

//Arduino
#include "Arduino.h"
#include "interface.h"

#include <MIDI.h>
#include "timerModule32.h"

//BSP
#include "adc_ext.h"
//#include "main_cubemx.h"

#define Serial Serial6

TaskStatus_t pxTaskStatusArray[5];

void taskConsolePrint(void)
{
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;
	char pcWriteBuffer[64];
	
	/* get num of tasks */
	uxArraySize = uxTaskGetNumberOfTasks();
    /* Generate raw status information about each task. */
    uxTaskGetSystemState( pxTaskStatusArray,
                               uxArraySize,
                               &ulTotalRunTime );

    /* For percentage calculations. */
    ulTotalRunTime /= 100UL;
	//ulTotalRunTime = 1; //fake it
	
    /* Avoid divide by zero errors. */
    if( ulTotalRunTime > 0 )
    {
        sprintf( pcWriteBuffer, " %-7s%10s%7s", "name", "epoch", "Load");
		Serial6.println(pcWriteBuffer);
		sprintf( pcWriteBuffer, "--------------------------");
		Serial6.println(pcWriteBuffer);
       /* For each populated position in the pxTaskStatusArray array,
       format the raw data as human readable ASCII data. */
       for( x = 0; x < uxArraySize; x++ )
       {
          /* What percentage of the total run time has the task used?
          This will always be rounded down to the nearest integer.
          ulTotalRunTimeDiv100 has already been divided by 100. */
          ulStatsAsPercentage =
                pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

          if( ulStatsAsPercentage > 0UL )
          {
             sprintf( pcWriteBuffer, " %-7s%10lu%6lu%%",
                               pxTaskStatusArray[ x ].pcTaskName,
                               pxTaskStatusArray[ x ].ulRunTimeCounter,
                               ulStatsAsPercentage );
          }
          else
          {
             /* If the percentage is zero here then the task has
             consumed less than 1% of the total run time. */
             sprintf( pcWriteBuffer, " %-7s%10lu%7s",
                               pxTaskStatusArray[ x ].pcTaskName,
                               pxTaskStatusArray[ x ].ulRunTimeCounter,
							   "<1%"							   );
          }
		Serial6.println(pcWriteBuffer);
       }
    }
}
//strMsg_t globoMsg = {0};
extern "C" void taskConsoleStart(void * argument)
{
	uint32_t nextUpdate = 0;
	
	while(1)
	{
		if(Serial6.available())
		{
			//Serial6.println(c);
			char c = (char)Serial6.read();
			switch(c)
			{
				case '\r':
				{
					Serial6.println();
					break;
				}
				case 'r':
				{
					Serial6.println();
					taskConsolePrint();
					break;
				}
				case '!':
				{
					Serial6.print("3");
					delay( 100 );
					Serial6.print("2");
					delay( 100 );
					Serial6.print("1");
					delay( 100 );

					strMsg_t * msg = new strMsg_t();

					//char buffer[32];
					//sprintf( buffer, "New: %p\n", &msg);
					//Serial6.println(buffer);

					msg->id = -1;
					msg->data[0] = '\0';
					if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
					{
						Serial6.println(".dud");
						delete msg;
					}
					break;
				}
				case 'H':
				{
					strMsg_t * msg = new strMsg_t();

					msg->id = 0;
					sprintf( msg->data, "Hello world!\n");
					if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
					{
						//TODO: error on send
						delete msg;
					}
					break;
				}
				case 't':
				{
					//Test delay times
					Serial6.println();
					Serial6.println(millis());
					vTaskDelay( 1000 );
					Serial6.println(millis());
					//Serial6.print(xTaskGetTickCount());
					break;
				}
				case 'l':
				{
					//Test delay times
					Serial6.print("L");
					delay( 333 );
					Serial6.print("O");
					delay( 333 );
					Serial6.print("A");
					delay( 333 );
					Serial6.print("D");
					//Serial6.print(xTaskGetTickCount());
					break;
				}
				default:
				{
					if(((c >= '0')&&(c <= '9'))||(c == '`'))
					{
						EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
						if((c != '`')&&(c != '8')&&(c != '9'))
						{
							uint8_t testKeyMask = 0x0001 << (c - '0');
							uxBits ^= testKeyMask;
							if( (uxBits >> (c - '0')) & 0x0001 )
							{
								xEventGroupSetBits(xTestEventGroup, testKeyMask );
							}
							else
							{
								xEventGroupClearBits(xTestEventGroup, testKeyMask );
							}

						}
						for(int i = 1; i <= 9; i++)
						{
							Serial6.print((uxBits >> i)&0x0001);
						}
						Serial6.println(uxBits&0x0001);
					}
					else
					{
						Serial6.print(".");
					}
				}

			}
		}

		uint32_t now = xTaskGetTickCount();
		if(now > nextUpdate)
		{
			nextUpdate = nextUpdate + 5000;
			//Thing that happens only so often -- don't block here
		}
		// Don't smash it!
		vTaskDelay( 10 );
	}
	
}
