#include "bsp.h"
#include "debugUtilities.h"
#include "tim.h"

uint32_t usTicks = 0;
uint32_t fastRunTimeTicks = 0;

//  The lock works like this:
//
//    When the interrupt fires, the lock is removed.  Now
//    the main free-wheeling loop can update the change to
//    the timerModules.  Once complete, the lock is replaced
//    so that it can't update more than once per firing
//    of the interrupt

uint8_t usTicksLocked = 1; //start locked out
uint32_t maxTimer = 60000000;
uint32_t maxInterval = 2000000;

bspTimerCallback_t timer3TickCallback;
bspTimerCallback_t sysTickCallbackPointer;

void bspRegisterSysTickCallback(bspTimerCallback_t cbFn)
{
	//This needs to save the os tick callback
	sysTickCallbackPointer = cbFn;
}

uint32_t adcUsCounter = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//traceWrite(VIOLET, 1);

	if(htim->Instance == TIM2)
	{
		//fast arbitrary run time ticker
		fastRunTimeTicks++;

		//10us resolution timer
		if(usTicks >= ( maxTimer + maxInterval ))
		{
			usTicks = usTicks - maxTimer;
		}
		else
		{
			usTicks = usTicks + 50;
		}
		usTicksLocked = 0;  //unlock
	}
	else if(htim->Instance == TIM3)
	{
		adcUsCounter += 100;
		if(adcUsCounter > 10000)
		{
			//Schedule an ADC conversion
			bspADCConvert();
			adcUsCounter = 0;
		}

		// Call into app
		if( timer3TickCallback != NULL )
		{
			timer3TickCallback();
		}
	}
	else if(htim->Instance == TIM4)
	{
		// Call into app
		//if( timer4TickCallback != NULL ) // Function defined locally
		{
			timer4TickCallback();
		}
	}
	else if (htim->Instance == TIM7)
	{
		HAL_IncTick();
	}
	//traceWrite(VIOLET, 0);
}

/* Defined in main.c. */
void vConfigureTimerForRunTimeStats( void )
{
	//This is called once, usually for HW timer config (done at bsp init)

}
