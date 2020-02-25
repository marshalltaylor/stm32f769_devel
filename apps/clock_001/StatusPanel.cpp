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


extern midi::MidiInterface<HardwareSerial> MIDI;

extern MidiClockDisplay Segments;

//extern BlinkerPanel mainPanel;
extern StatusPanel statusPanel;

void StatusPanel::BeatCallback(MidiClock * caller)
{
	char buffer[5];
	// Screen logic
	switch(caller->getState())
	{
		case Playing:
		case Paused:
		{
			sprintf( buffer, "%2d%2d", (int)caller->ticksToMeasures(caller->ticks), (int)caller->ticksToQuarterNotes(caller->ticks) );
			Segments.displayDrawClockNums(buffer);
		}
		break;
		default:
		break;
	}

	// LED logic
	switch(caller->getState())
	{
		case Stopped:
		case Playing:
		{
			statusPanel.beatFlag.setFlag();
			statusPanel.beatFlag.clearFlag();
			statusPanel.playFlag.setFlag();
			statusPanel.playFlag.clearFlag();
		}
		break;
		default:
		break;
	}

}

void StatusPanel::TickCallback(MidiClock * caller)
{
	char buffer[5];
	switch(caller->getState())
	{
		case Stopped:
		{
			MIDI.sendRealTime(midi::Clock);
			sprintf( buffer, "----" );
			Segments.displayDrawClockNums(buffer);
		}
		break;
		case OutputOff:
		{
			sprintf( buffer, "    " );
			Segments.displayDrawClockNums(buffer);
		}
		default:
		case Paused:
		case Playing:
			MIDI.sendRealTime(midi::Clock);
		break;
		break;
	}

}

StatusPanel::StatusPanel( void )
{
	ledBeat.setHardware(new ArduinoDigitalOut( A5 ), 1);
	add( &ledBeat );
	ledPlay.setHardware(new ArduinoDigitalOut( D34 ), 1);
	add( &ledPlay );

	reset();
	//freshenComponents( 1 );
}

void StatusPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	ledBeat.setState(LEDOFF);
	ledPlay.setState(LEDOFF);
	beatLedState = BeatLedStateInit;
	// Construction order screws this up... it shouldn't be here anyway
	//ClockSocket->SwitchMidiClock(NULL);
}

void StatusPanel::tickStateMachine( int usTicksDelta )
{
	beatTimeKeeper.uIncrement( usTicksDelta );
	playTimeKeeper.uIncrement( usTicksDelta );
	
	// I don't think LEDs use the value here, normally you would pass msTicksDelta
	freshenComponents( 1 );
	
	//***** PROCESS THE LOGIC *****//
	MidiClock * clock = ClockSocket->getSocketedClock();
	
	//Now do the states.
	switch( beatLedState )
	{
	case BeatLedStateInit:
		beatLedState = BeatLedStateOff;
		break;
	case BeatLedStateOff:
		if( beatFlag.serviceRisingEdge() )
		{
			beatLedState = BeatLedStateOn;
			ledBeat.setState(LEDON);
			beatTimeKeeper.uClear();
		}
		break;
	case BeatLedStateOn:
		if( beatTimeKeeper.uGet() > 10000 )
		{
			beatLedState = BeatLedStateOff;
			ledBeat.setState(LEDOFF);
		}
		break;
	default:
		beatLedState = BeatLedStateInit;
		break;
	}

	switch( playLedState )
	{
	case PlayLedStateInit:
		playLedState = PlayLedStateOff;
		break;
	case PlayLedStateOff:
		if( clock->getState() == Playing )
		{
			playLedState = PlayLedStateOn;
			localPrintf(" to PlayLedStateOn\n");
			ledPlay.setState(LEDON);
		}
		else if( playFlag.serviceRisingEdge() )
		{
			if( clock->getState() == Paused )
			{
				playLedState = PlayLedStateBlink;
				localPrintf(" to PlayLedStateBlink\n");
				ledPlay.setState(LEDON);
				playTimeKeeper.uClear();
			}
		}

		break;
	case PlayLedStateOn:
		if( clock->getState() == Stopped )
		{
			playFlag.serviceRisingEdge();
			playLedState = PlayLedStateOff;
			localPrintf(" to PlayLedStateOff\n");
			ledPlay.setState(LEDOFF);
		}
		break;
	case PlayLedStateBlink:
		if( playTimeKeeper.uGet() > 600 )
		{
			playLedState = PlayLedStateOff;
			localPrintf(" to PlayLedStateOff\n");
			ledPlay.setState(LEDOFF);
		}
		break;
	default:
		playLedState = PlayLedStateInit;
		break;
	}

}
