#include "App.h"
#include "sketch.h"

int AppEntry(void)
{
	setup();
	
	/* We should never get here as control is now taken by the scheduler */
	while(1)
	{
		loop(); // Legacy execution
	}
}
