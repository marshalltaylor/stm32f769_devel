#ifndef MIDICLOCKDISPLAY_H
#define MIDICLOCKDISPLAY_H
#include <stdint.h>
#include "SegmentVideo.h"

typedef enum displayStates
{
	init,
	idle,
	fadeIn,
	waitForFadeIn,
	waitForTimeout,
	fadeOut,
	waitForFadeOut
} displayStates_t;

class MidiClockDisplay : public SegmentVideo
{
public:
	MidiClockDisplay(void){};
	
	//Graphics
	void displayDrawClockNums( const char * input );
	void displayDrawValue( const char * input );
	void setPlayIndicator( void );
	void clearPlayIndicator( void );
	void toggleClockColon(void);
	
	void showNewValue( const char * input );
	void processEffects(void);
	bool debugNoise = false;
	
	//State machine
	void tickValueStateMachine( uint32_t sysTime ); //Give in milliseconds
	uint8_t getValueState( void );
	
private:
	uint32_t startTime = 0;
	bool newValueRequested = false;
	bool restartTimer = true;
	const char * currentValue;
	const char * nextValue;
	const char blankValue[4] = "   ";

	displayStates_t displayState;
};

#endif