#ifndef MIDI_47EFFECTS_H
#define MIDI_47EFFECTS_H

#include "src/MIDI.h"
#include "src/midi_Defs.h"
#include "bsp.h"

class HardwareSerial
{
protected:
public:
	HardwareSerial(void);
	void initPort(comPort_t);
//	void begin(unsigned long baud) { begin(baud, 0); }
	void begin(unsigned long baud);

	//	void init(UartInstance* HAL_UART);
//    void end();
	virtual int available(void);
    virtual int peek(void);
	virtual int read(void);
//    int availableForWrite(void);
//    virtual void flush(void);
	virtual size_t write(uint8_t);
//    inline size_t write(unsigned long n) { return write((uint8_t)n); }
//    inline size_t write(long n) { return write((uint8_t)n); }
//    inline size_t write(unsigned int n) { return write((uint8_t)n); }
//    inline size_t write(int n) { return write((uint8_t)n); }
//    operator bool() { return true; }
//
private:
	comPortInterface_t bspSerialMidiObj;
//    uint8_t _config;
};

extern HardwareSerial TestSerial;

#endif
