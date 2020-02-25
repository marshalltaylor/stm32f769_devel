#include "uCModules.h"
#include "KnobPanel.h"
#include "bsp.h"

KnobPanel::KnobPanel( void )
{
	myKnob.setHardware(new ArduinoAnalogIn( A0 ));
	add( &myKnob );
	
	state = KnobPanelScope::init;
}

void KnobPanel::reset( void )
{
	state = KnobPanelScope::init;
	
}

void KnobPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	KnobPanelScope nextState = state;
	switch( state )
	{
	case KnobPanelScope::init:
		nextState = KnobPanelScope::run;
		break;
	case KnobPanelScope::run:
		if( myKnob.serviceChanged() )
		{
			bspPrintf("knob: %d\n", myKnob.getState());
		}
		break;
	default:
		nextState = KnobPanelScope::init;
		break;
	}
	if(state != nextState)
	{
		bspPrintf("[KNOB    ] State %d -> %d\n", state, nextState);
		state = nextState;
	}

}
