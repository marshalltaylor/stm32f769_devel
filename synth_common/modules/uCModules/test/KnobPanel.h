//Header
#ifndef KNOBPANEL_H_INCLUDED
#define KNOBPANEL_H_INCLUDED

#include "uCModules.h"

enum class KnobPanelScope
{
	init,
	run
};

class KnobPanel : public Panel
{
public:
	KnobPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	
private:
	//Internal Panel Components
	Windowed10BitKnob myKnob;

	//State machine stuff  
	KnobPanelScope state;

};



#endif