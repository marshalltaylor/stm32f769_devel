/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

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
#include "MidiClockDisplay.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

extern MidiClockDisplay Segments;
//extern char value1Str[8];

uint8_t peekValue = 128;
static comPortInterface_t console;
TaskStatus_t pxTaskStatusArray[5];


void delay(uint32_t delayInput);

/* Functions -----------------------------------------------------------------*/
void delay(uint32_t delayInput)
{
	uint32_t targetTicks = millis() + delayInput; //OK for this to roll
	while(millis() > targetTicks)
	{
		//if we rolled, wait until millis rolls
	}
	while(millis() < targetTicks)
	{
		//nop
	}
}

void taskConsolePrintStats(void)
{
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;

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
		localPrintf(" %-7s%10s%7s\n", "name", "epoch", "Load");
		localPrintf("--------------------------\n");
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
				localPrintf(" %-7s%10lu%6lu%%\n",
							pxTaskStatusArray[ x ].pcTaskName,
							pxTaskStatusArray[ x ].ulRunTimeCounter,
							ulStatsAsPercentage
							);
			}
			else
			{
			 /* If the percentage is zero here then the task has
			 consumed less than 1% of the total run time. */
			 localPrintf(" %-7s%10lu%7s\n",
						pxTaskStatusArray[ x ].pcTaskName,
						pxTaskStatusArray[ x ].ulRunTimeCounter,
						"<1%"
						);
			}
		}
	}
}

void taskConsolePrintHelp(void)
{
	localPrintf("Segment Video Test Console.\n");
	localPrintf(" h: help\n");
	localPrintf(" i: increment value\n");
	localPrintf(" p: start peeking\n");
	localPrintf(" 0 - 9: graphical tests\n");
}

//strMsg_t globoMsg = {0};
extern "C" void taskConsoleStart(void * argument)
{
	uint32_t nextUpdate = 0;
#ifdef USE_LOGGING
	consoleDebug.setStamp("Console", 7);
	consoleDebug.setMode(LOG_MODE_DEFAULT);
#endif

	bspGetSerialFunctions(COM0, &console);

	taskConsolePrintHelp();
	while(1)
	{
		if(console.bytesAvailable())
		{
			char c = (char)console.read();
			switch(c)
			{
				case '\r':
				{
					localPrintf("\n");
					break;
				}
				case 'r':
				{
					localPrintf("\n");
					taskConsolePrintStats();
					break;
				}
				case 'h':
				{
					taskConsolePrintHelp();
					break;
				}
				case 'i':
				{
					localPrintf("Increment 'peek' value\n");
					peekValue++;
					char buffer[5];
					sprintf(buffer, "%03d", peekValue);
					Segments.displayDrawValue(buffer);
					break;
				}
				case 'p':
				{
					localPrintf("Start peeking\n");
					char buffer[5];
					sprintf(buffer, "%03d", peekValue);
					Segments.showNewValue(buffer);
					break;
				}
				case '!':
				{
					localPrintf("3");
					delay(300);
					localPrintf("2");
					delay(300);
					localPrintf("1");
					delay(300);

					strMsg_t * msg = new strMsg_t();

					msg->id = -1;
					msg->data[0] = '\0';
					if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
					{
						localPrintf(".dud");
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
					localPrintf("\n");
					localPrintf("Start time  %dms\n", millis());
					vTaskDelay( 1000 );
					localPrintf("Stop time   %dms\n", millis());
					//localPrintf("tick count     %dms\n", xTaskGetTickCount());
					break;
				}
				case 'l':
				{
					//Test delay times
					localPrintf("L");
					delay( 333 );
					localPrintf("O");
					delay( 333 );
					localPrintf("A");
					delay( 333 );
					localPrintf("D");
					break;
				}
				default:
				{
					if(((c >= '0')&&(c <= '9'))||(c == '`'))
					{
						EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
						uint8_t testKeyMask = 0;
						if((c != '`')&&(c != '8')&&(c != '9'))
						{
							testKeyMask = 0x0001 << (c - '0');
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
							localPrintf("%d", (uxBits >> i)&0x0001);
						}
						localPrintf("%d\n", uxBits&0x0001);
						
						//Optionally do something with key like regular state
						if(c == '2')
						{
							if(uxBits & testKeyMask)
							{
								consoleDebug.setMode(LOG_MODE_AUTO);
							}
							else
							{
								consoleDebug.setMode(LOG_MODE_DEFAULT);
							}
						}
					}
					else
					{
						localPrintf(".");
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
		// Clock segment driver
		vTaskDelay( 5 );
		Segments.tickValueStateMachine(millis());

		Segments.processEffects();
		Segments.writeNextFrame();
		
	}
	
}
