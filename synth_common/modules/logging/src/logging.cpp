#include <string.h>

#include "logging.h"

Logging::Logging(void)
{
	pPrintf = bspGetSerialConsolePrintf();
	pVPrintf = bspGetSerialConsoleVPrintf();
}

void Logging::printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if(pVPrintf)
	{
		if(logMode == LOG_MODE_AUTO)
		{
			pPrintf("%s", stamp);
		}

		pVPrintf(fmt, args);
	}

	va_end(args);
}

void Logging::setVPrintf(bspVPrintf_t function)
{
	pVPrintf = function;
}

void Logging::setStamp(const char * data, uint8_t len)
{
	if(len > LOGGING_STAMP_MAX_LEN)
	{
		len = LOGGING_STAMP_MAX_LEN;
	}
	
	int indexOut = 0;
	stamp[indexOut++] = '[';
	stamp[indexOut++] = ' ';
	for(int indexIn = 0; indexIn < LOGGING_STAMP_MAX_LEN; indexIn++)
	{
		if(indexIn < len)
		{
			stamp[indexOut++] = data[indexIn];
		}
		else
		{
			stamp[indexOut++] = ' ';
		}
	}
	stamp[indexOut++] = ' ';
	stamp[indexOut++] = ']';
	stamp[indexOut++] = ' ';
	stamp[indexOut++] = '\0';
}

void Logging::setMode(loggingMode_t mode)
{
	logMode = mode;
}
