#include "bsp.h"
#include "SegmentVideo.h"

//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

SegmentFrameBuffer::SegmentFrameBuffer(void){
	for(int i = 0; i < 11; i++)
	{
		onBuffer[0][i] = 0xF + i;
		offBuffer[0][i] = 0xF + i;
	}
}

uint8_t testData[11] = {1,2,0,0,0xFF,0xF0,0xFF,8,9,10,11};
bool SegmentFrameBuffer::read( BufferChannels * retVar )
{
	retVar->onBufferPtr = &(onBuffer[outputPtr][0]);
	retVar->offBufferPtr = &(offBuffer[outputPtr][0]);

	if(buffersAreNull)
	{
		return false;
	}
	
	if( ((outputPtr == FB_FIFO_SIZE - 1)&&( nextToWrite == 0 )) || (outputPtr + 1 == nextToWrite) )
	{
		// no new data, special case
		return true;
	}

#if 0
	int i;
	localPrintf("instance: 0x%08X, out = %d, next = %d\n", (uint32_t)this, outputPtr, nextToWrite);
	localPrintf("   ON DATA:  0x[ ");
	for(i = 0; i < 11; i++)
	{
		localPrintf("%02X ", retVar->onBufferPtr[i]);
	}
	localPrintf("]\n");
	localPrintf("   OFF DATA: 0x[ ");
	for(i = 0; i < 11; i++)
	{
		localPrintf("%02X ", retVar->offBufferPtr[i]);
	}
	localPrintf("]\n");
#endif

	outputPtr++;
	if( outputPtr >= FB_FIFO_SIZE )
	{
		outputPtr = 0;
	}	
	return true;
}

// Put a new frame into the frame buffer
void SegmentFrameBuffer::write(const uint8_t * onData, const uint8_t * offData)
{
	buffersAreNull = false;
	if(nextToWrite == outputPtr)
	{
		// full
		return;
	}
	for(int i = 0; i < 11; i++)
	{
		onBuffer[nextToWrite][i] = onData[i];
		offBuffer[nextToWrite][i] = offData[i];
		//onBuffer[nextToWrite][i] = testData[i];
		//offBuffer[nextToWrite][i] = testData[i];
		//onBuffer[nextToWrite][i] = 0x03;
		//offBuffer[nextToWrite][i] = 0x03;
	}
	nextToWrite++;
	if( nextToWrite >= FB_FIFO_SIZE )
	{
		nextToWrite = 0;
	}
	//Serial6.print("outputPtr = ");
	//Serial6.println(outputPtr);
	//Serial6.print("nextToWrite = ");
	//Serial6.println(nextToWrite);
}

bool SegmentFrameBuffer::ready( void )
{
	int16_t length;
	if( outputPtr > nextToWrite )
	{
		length = (nextToWrite + FB_FIFO_SIZE) - outputPtr - 1;
	}
	else
	{
		length = nextToWrite - outputPtr - 1;
	}
	return ( FB_FIFO_SIZE - length > FB_WATERMARK );
}

bool SegmentFrameBuffer::empty( void )
{
	int16_t length;
	if( outputPtr > nextToWrite )
	{
		length = (nextToWrite + FB_FIFO_SIZE) - outputPtr - 1;
	}
	else
	{
		length = nextToWrite - outputPtr - 1;
	}
	if (length == 0)
	{
		return true;
	}
	return false;
}

// Mixer

void SegmentVideo::writeNextFrame(void)
{
	BufferChannels nextValueMaskFrame;
	BufferChannels nextFGFrame;
	BufferChannels nextNoiseFrame;

	bool skipFrame = false; //ignore null data
	if(!valueMask_layer.read(&nextValueMaskFrame))
	{
		//fail
		skipFrame = true;
	}
	if(!fg_layer.read(&nextFGFrame))
	{
		//fail
		skipFrame = true;
	}
	if(!noise_layer.read(&nextNoiseFrame))
	{
		//fail
		skipFrame = true;
	}
	
	if(skipFrame)
	{
		//Bad init read from one of them
		return;
	}
	
	for(int i = 0; i < 11; i++)
	{
		outputFrame[i] = textBitmap[i];
		outputFrame[i] &= ~nextValueMaskFrame.offBufferPtr[i];
		outputFrame[i] |= nextValueMaskFrame.onBufferPtr[i];
		//outputFrame[i] = nextFGFrame.onBufferPtr[i];
		outputFrame[i] &= ~nextNoiseFrame.offBufferPtr[i];
		outputFrame[i] |= nextNoiseFrame.onBufferPtr[i];
	}	
	bspSPISegmentWrite(outputFrame);
}
