#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp.h"

#include "midi47fx.h"

//functions
HardwareSerial::HardwareSerial(void)
{
}

void HardwareSerial::initPort(comPort_t port)
{
	if(port >= NUM_COM_PORTS)
	{
		return;
	}
	bspGetSerialFunctions(port, &bspSerialMidiObj);
}

void HardwareSerial::begin(unsigned long baud)
{
}

int HardwareSerial::available(void)
{
	if( bspSerialMidiObj.bytesAvailable != NULL )
	{
		return bspSerialMidiObj.bytesAvailable();
	}
	return 0;
}

int HardwareSerial::peek(void)
{
	if( bspSerialMidiObj.peek != NULL )
	{
		return bspSerialMidiObj.peek();
	}
	return 0;
}

int HardwareSerial::read(void)
{
	if( bspSerialMidiObj.read != NULL )
	{
		return bspSerialMidiObj.read();
	}
	return 0;
}

size_t HardwareSerial::write(uint8_t c)
{
	if( bspSerialMidiObj.write != NULL )
	{
		bspSerialMidiObj.write(c);
	}
	return 1;
}
