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


//#include "midi_Defs.h"

extern midi::MidiInterface<HardwareSerial> MIDI;

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;
extern StatusPanel statusPanel;
extern MidiClockDisplay Segments;

const uint8_t FadePixelAllow[11] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t FadePixelDeny[11] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	
BlinkerPanel::BlinkerPanel( void )
{
	buttonSelect.setHardware(new ArduinoDigitalIn( D24 ), 1);
	add( &buttonSelect );
	button4.setHardware(new ArduinoDigitalIn( D25 ), 1);
	add( &button4 );
	button3.setHardware(new ArduinoDigitalIn( D26 ), 1);
	add( &button3 );
	button2.setHardware(new ArduinoDigitalIn( D27 ), 1);
	add( &button2 );
	button1.setHardware(new ArduinoDigitalIn( D28 ), 1);
	add( &button1 );
	stop.setHardware(new ArduinoDigitalIn( D29 ), 1);
	add( &stop );
	play.setHardware(new ArduinoDigitalIn( D30 ), 1);
	add( &play );

	led1.setHardware(new ArduinoDigitalOut( D32 ), 1);
	add( &led1 );
	led2.setHardware(new ArduinoDigitalOut( D31 ), 1);
	add( &led2 );
	led3.setHardware(new ArduinoDigitalOut( D33 ), 1);
	add( &led3 );
	led4.setHardware(new ArduinoDigitalOut( D10 ), 1);
	add( &led4 );

	knob1.setHardware(new ArduinoAnalogIn( A1 ));
	add( &knob1 );
	knob1.setLowerKnobVal(250);
	knob1.setUpperKnobVal(1014);
	knob1.setLowerIntVal(0);
	knob1.setUpperIntVal(100);
	knob1.setSamplesAveraged(10);
	knob1.setWindow(100);

	knob3.setHardware(new ArduinoAnalogIn( A0 ));
	add( &knob3 );
	knob3.setLowerKnobVal(10);
	knob3.setUpperKnobVal(1014);
	knob3.setLowerIntVal(0);
	knob3.setUpperIntVal(255);
	knob3.setSamplesAveraged(10);
	knob3.setWindow(100);

	knobTempo.setHardware(new ArduinoAnalogIn( A2 ));
	add( &knobTempo );
	knobTempo.setLowerKnobVal(10);
	knobTempo.setUpperKnobVal(1014);
	knobTempo.setLowerIntVal(40);
	knobTempo.setUpperIntVal(208);
	knobTempo.setSamplesAveraged(10);
	knobTempo.setWindow(100);

	reset();

}

void BlinkerPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	led1.setState(LEDOFF);
	led2.setState(LEDOFF);
	led3.setState(LEDOFF);
	led4.setState(LEDOFF);
	state = PInit;
	
	// timeMaster = true; //overridden by switchTo...
	switchToInternalClock();
	
	currentBPM = knobTempo.getAsInt16() * 1000;
	targetBPM = currentBPM;
	glideRate = knob1.getAsInt16() * 1000;
	intMidiClock.setBPM(currentBPM / 1000);
	
}

void BlinkerPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	//***** PROCESS THE LOGIC *****//
	MidiClock * clock = statusPanel.ClockSocket->getSocketedClock();
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		//Can't be running, if button pressed move on
		reset();
		nextState = PRunning;
		break;
	case PRunning:
		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

	if( button1.serviceRisingEdge() )
	{
		localPrintf("Button1");
		glideEnabled = !glideEnabled;
		if(glideEnabled)
		{
			led1.setState(LEDON);
		}
		else
		{
			led1.setState(LEDOFF);
		}
	}
	if( button2.serviceRisingEdge() )
	{
		localPrintf("Button2");
		led2.toggle();
		//emergencyRestart();
		Segments.debugNoise = !led2.getState();
	}
	if( button3.serviceRisingEdge() )
	{
		localPrintf("Button3");
		led3.toggle();
	}
	if( button4.serviceRisingEdge() )
	{
		localPrintf("Button4");
		if(!timeMaster)
		{
//			clock->stop();
			switchToInternalClock();
		}
		else
		{
			switchToExternalClock();
		}
	}
	if( buttonSelect.serviceRisingEdge() )
	{
		localPrintf("Select");
	}
	if( play.serviceRisingEdge() )
	{
		localPrintf("play");
		
		if( timeMaster )
		{
			switch( clock->getState() )
			{
				case OutputOff:
				case Stopped:
				{
					clock->setState(Playing);
					clock->setTickCount(0);
					MIDI.sendRealTime(midi::Start);
				}
				break;
				case Playing:
				{
					clock->setState(Paused);
					MIDI.sendRealTime(midi::Stop);
				}
				break;
				case Paused:
				{
					clock->setState(Playing);
					MIDI.sendRealTime(midi::Continue);
				}
				break;
				default:
				break;

			}
		}
		else
		{
			//Let midi do it
		}
	}
	if( stop.serviceRisingEdge() )
	{
		if( timeMaster )
		{
			switch( clock->getState() )
			{
			case Paused:
			case Playing:
			{
				clock->setState(Stopped);
				MIDI.sendRealTime(midi::Stop);
			}
			break;
			case Stopped:
			{
				clock->setState(OutputOff);
				//just in case
				MIDI.sendRealTime(midi::Stop);
			}
			break;
			default:
			break;
			}
		}
		else
		{
			//Let midi do it
		}
	}
	
	char buffer[8];
	int16_t newValue;
	if( knob1.serviceChanged() )
	{
		localPrintf("knob1: %d\n", knob1.getState());
		localPrintf(" Val: %d\n", bspIOPinReadAnalog(A1));

		newValue = knob1.getAsInt16();
		sprintf( buffer, "%3d", newValue );
		if(pLastKnobShown == &knob1)
		{
			Segments.displayDrawValue(buffer);
		}
		else
		{
			Segments.showNewValue(buffer);
		}
		pLastKnobShown = &knob1;

		glideRate = newValue * 1000;
	}
	if( knob3.serviceChanged() )
	{
		localPrintf("knob3: %d\n", knob3.getState());
		localPrintf(" Val: %d\n", bspIOPinReadAnalog(A2));

		newValue = knob3.getAsInt16();
		sprintf( buffer, "%3d", newValue );
		if(pLastKnobShown == &knob3)
		{
			Segments.displayDrawValue(buffer);
		}
		else
		{
			Segments.showNewValue(buffer);
		}
		pLastKnobShown = &knob3;

		//No real action
		}
	if( knobTempo.serviceChanged() )
	{
		localPrintf("knobTempo: %d\n", knobTempo.getState());
		localPrintf(" Val: %d\n", bspIOPinReadAnalog(A2));

		newValue = knobTempo.getAsInt16();
		sprintf( buffer, "%3d", newValue );
		if(pLastKnobShown == &knobTempo)
		{
			Segments.displayDrawValue(buffer);
		}
		else
		{
			Segments.showNewValue(buffer);
		}
		pLastKnobShown = &knobTempo;

		targetBPM = newValue * 1000;
	}
	
	int32_t scaledGlide = glideRate / 100;
	if( glideEnabled )
	{
		if( currentBPM > targetBPM )
		{
			currentBPM -= scaledGlide;
			if( currentBPM < targetBPM )
			{
				currentBPM = targetBPM;
			}
			intMidiClock.setBPM(currentBPM / 1000);
			localPrintf("(-)currentBPM: %d\n", currentBPM);
			localPrintf("   targetBPM: %d\n", targetBPM);

		}
		if( currentBPM < targetBPM )
		{
			currentBPM += scaledGlide;
			if( currentBPM > targetBPM )
			{
				currentBPM = targetBPM;
			}
			intMidiClock.setBPM(currentBPM / 1000);
			localPrintf("(+)currentBPM: %d\n", currentBPM);
			localPrintf("   targetBPM: %d\n", targetBPM);
		}
	}
	else
	{
		if( currentBPM != targetBPM )
		{
			currentBPM = targetBPM;
			intMidiClock.setBPM(currentBPM / 1000);
			localPrintf("(S)currentBPM: %d\n", currentBPM);
			localPrintf("   targetBPM: %d\n", targetBPM);
		}
	}
}

void BlinkerPanel::switchToInternalClock( void )
{
	led4.setState(LEDON);
	timeMaster = 1;
	clockSocket.SwitchMidiClock(&intMidiClock);
}

void BlinkerPanel::switchToExternalClock( void )
{
	led4.setState(LEDOFF);
	timeMaster = 0;
	clockSocket.SwitchMidiClock(&extMidiClock);
}
