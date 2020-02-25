/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "midi47fx.h"
#include "logging.h"
#include "uCModules.h"

#include "SegmentVideo.h"

/* Includes -- App -----------------------------------------------------------*/
#include "App.h"

#include "midiTime.h"
#include "StatusPanel.h"
#include "BlinkerPanel.h"


/* References ----------------------------------------------------------------*/
//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf dummy.printf
Logging dummy;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif



MidiClockDisplay Segments;

MidiClock extMidiClock;
MidiClock intMidiClock;
MidiClockSocket clockSocket;

//TODO: Fix this for new serial type stuff that hasn't been finished yet....
//MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

HardwareSerial TestSerial;
MIDI_CREATE_INSTANCE(HardwareSerial, TestSerial,  MIDI);



BlinkerPanel mainPanel;
StatusPanel statusPanel;

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 1000000 ); //1 second
TimerClass32 mainPanelTimer( 10000 );
TimerClass32 statusPanelTimer( 400 );
TimerClass32 segmentVideoTimer( 5000 );

#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

void handleClock( void )
{
	extMidiClock.incrementTick();
	//Serial6.println("Clock");
}

void handleStart( void )
{
	extMidiClock.setTickCount(0);
	extMidiClock.setState(Playing);
}

void handleContinue( void )
{
	extMidiClock.setState(Playing);
}

void handleStop( void )
{
	extMidiClock.setState(Stopped);
}

	
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	//digitalWrite(D6, 0);
	//Serial6.print("pitch: ");
	//Serial6.println(pitch);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	//digitalWrite(D6, 1);
}

void hwTimerCallback(void)
{
	// Things!
	intMidiClock.incrementTime_uS(100);
}

//Defined in bspTime.h
extern uint8_t usTicksLocked;
extern uint32_t usTicks;

int AppEntry(void)
{
	TestSerial.initPort(COM1);	
	//Serial6.println("OK");
	intMidiClock.setBPM(100);
	// Write our function address into the hw timer
	//timer3TickCallback = &MidiClock::hwTimerCallback;
	timer3TickCallback = hwTimerCallback;
	//Go to fresh state
	mainPanel.reset();
	statusPanel.reset();
	
	clockSocket.SetBeatCallback(statusPanel.BeatCallback);
	clockSocket.SetTickCallback(statusPanel.TickCallback);
	clockSocket.SwitchMidiClock(&extMidiClock);
	statusPanel.ClockSocket = &clockSocket;

	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleContinue(handleContinue);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
	
	//uint8_t AllZeros[11];
	////uint8_t AllOnes[11];
	//for(int i = 0; i < 11; i++)
	//{
	//	AllZeros[i] = 0x00;
	//	//AllOnes[i] = 0xFF;
	//}
	//Segments.valueMask_layer.write(AllZeros, AllZeros);
	//Segments.fg_layer.write(AllZeros, AllZeros);
	//Segments.noise_layer.write(AllZeros, AllZeros);
	
	
	
	
	
	
	//Main loop
	while(1)
	{
		MIDI.read();
		//if(Serial6.available())
		//{
		//	Serial6.println((char)Serial6.read());
		//}
		if( usTicksLocked == 0 )
		{
			mainPanelTimer.update(usTicks);
			debugTimer.update(usTicks);
			statusPanelTimer.update(usTicks);
			segmentVideoTimer.update(usTicks);
			//Done?  Lock it back up
			usTicksLocked = 1;
		}  //The ISR will unlock.
	
		if(mainPanelTimer.flagStatus() == PENDING)
		{
			mainPanel.tickStateMachine(10);
			Segments.tickValueStateMachine(millis());
		}
	
		if(statusPanelTimer.flagStatus() == PENDING)
		{
			statusPanel.tickStateMachine(400);
			//intMidiClock.incrementTime_uS(400);
			intMidiClock.service();
		}
		
		if(debugTimer.flagStatus() == PENDING)
		{
			localPrintf("__DEBUG______\nintPlayState = %d, extPlayState = %d\nbeatLedState = %d, playLedState = %d\nFreeStack() = %d\n\n", intMidiClock.getState(), extMidiClock.getState(), statusPanel.getBeatLedState(), statusPanel.getPlayLedState(), FreeStack());
		}
		if(segmentVideoTimer.flagStatus() == PENDING)
		{
			Segments.processEffects();
			Segments.writeNextFrame();
		}
	}

	return -1;
}
