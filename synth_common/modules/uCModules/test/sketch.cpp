#include "bsp.h"

#include "uCModules.h"

#include "BlinkerPanel.h"
BlinkerPanel myCustomPanel;

#include "KnobPanel.h"
KnobPanel myKnobPanel;


uint32_t debugCounter = 0;

void hwTimerCallback(void)
{
	// Things!
	//was 100us for midi
}

#ifdef __cplusplus
extern "C" {
#endif


void setup(void)
{
	// Start the fake Arduino interface stuff
	// interface_init();

	//pinMode(D6, OUTPUT);
	//Serial2.begin(9600, 6);
	//Serial6.begin(12345, 1);
	//delay(2000);
	//Serial2.println("ok");
	//Serial6.println("OK");
	// Write our function address into the hw timer
	//timer3TickCallback = &MidiClock::hwTimerCallback;
	timer3TickCallback = hwTimerCallback;

	//Go to fresh state
	myCustomPanel.reset();
	myKnobPanel.reset();

	
}

//Set LOOP_DELAY to length of time between ticks of everything (in ms)
#define LOOP_DELAY 10

void loop()
{
	//Tick the machine
	myCustomPanel.tickStateMachine(LOOP_DELAY);
	myKnobPanel.tickStateMachine(LOOP_DELAY);
	
//	//Delay the loop
//	delay(LOOP_DELAY);
//	
	//Increment a counter for debug stuff
	uint32_t timeStart = millis();
	while(millis() < timeStart + 10);
	
}

#ifdef __cplusplus
}
#endif

