#include "Arduino.h"
#include "interface.h"
#include <MIDI.h>
#include "adc_ext.h"

#include "timerModule32.h"

#define Serial Serial6

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 1000000 ); //1 second
TimerClass32 mainPanelTimer( 10000 );
TimerClass32 statusPanelTimer( 400 );
TimerClass32 segmentVideoTimer( 5000 );


void hwTimerCallback(void)
{
	// Things!
//	intMidiClock.incrementTime_uS(100);
//	intMidiClock.service();
}

#ifdef __cplusplus
extern "C" {
#endif

void setup(void)
{
	// Start the fake Arduino interface stuff
	interface_init();

	//pinMode(D6, OUTPUT);
	Serial2.begin(9600, 6);
	Serial6.begin(12345, 1);
	//delay(2000);
	//Serial2.println("ok");
	Serial6.println("OK");

	// Write our function address into the hw timer
	//timer3TickCallback = &MidiClock::hwTimerCallback;
	timer3TickCallback = hwTimerCallback;

	//Go to fresh state
	
}

void taskMidi(void)
{
	//MIDI.read();
}

#ifdef __cplusplus
}
#endif

