/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "main.h"
#include "os.h"

#include "MidiClockDisplay.h"

MidiClockDisplay Segments;


/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private function prototypes -----------------------------------------------*/

int main(void)
{
	bspInit();

	//Init and run operating system.  This starts all tasks
	osInit();

	/* We should never get here as control is now taken by the scheduler */
	while (1)
	{
	}
}
