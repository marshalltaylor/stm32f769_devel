#include <stddef.h>

#include "PanelDataObjects.h"
#include "uCModulesPrivate.h"

DataObject::DataObject( void )
{
	size = -1;
	data = NULL;
}

ButtonDataObject::ButtonDataObject( void ){
	size = 1;
	data = new uint8_t[size];
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", +");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
}

ButtonDataObject::~ButtonDataObject( void ){
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", -");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
	delete[] data;
}

LedDataObject::LedDataObject( void ){
	size = 1;
	data = new uint8_t[size];
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", +");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
}

LedDataObject::~LedDataObject( void ){
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", -");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
	delete[] data;
}

KnobDataObject::KnobDataObject( void )
{
	size = 2;
	data = new uint8_t[size];
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", +");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
}

KnobDataObject::~KnobDataObject( void )
{
#ifdef ARDUINO_PRINT_DEBUG
//Serial.print((uint32_t)&(*this), HEX);
//Serial.print(", -");
//Serial.print(sizeof(data));
//Serial.print("B @");
//Serial.println((uint32_t)&(*data), HEX);
#endif
	delete[] data;
}


//dumpObject lists a DataObject's contents, plus the contents of it's contained data
void dumpObject( DataObject * inputObjectPtr )
{
	//dbg.printf("Obj Addr: 0x08X", (uint32_t)&(*inputObjectPtr));
	uint8_t * bytePtr = (uint8_t *)inputObjectPtr;
	for( uint32_t i = 0; i < sizeof(*inputObjectPtr); i++ )
	{
		
		//dbg.printf("0x02X, ", *bytePtr);
		bytePtr++;
	}
	//dbg.printf("\n");
	
	//dbg.printf("Containted data: ");
	bytePtr = (uint8_t *)inputObjectPtr->data;
	for( uint32_t i = 0; i < inputObjectPtr->size; i++ )
	{
		//dbg.printf("0x02X, ", *bytePtr);
		bytePtr++;
	}
	//dbg.printf("\n");

}
