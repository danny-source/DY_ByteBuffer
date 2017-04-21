/*
Arduino Byte circle buffer
*/

#ifndef DYByteBuffer_h
#define DYByteBuffer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef uint8_t
#include <stdint.h>
#endif

class DYByteBuffer
{
public:
	DYByteBuffer();

	// This method initializes the datastore of the buffer to a certain sizem the buffer should NOT be used before this call is made
	void init(uint16_t size);

	void init(uint8_t *buf,uint16_t size);

	// This method resets the buffer into an original state (with no data)
	void clear();

	// This releases resources for this buffer, after this has been called the buffer should NOT be used
	void deAllocate();

	// Returns how much space is left in the buffer for more data
	uint16_t getSize();

	// Returns the maximum capacity of the buffer
	uint16_t getCapacity();

	// This method returns the byte that is located at index in the buffer but doesn't modify the buffer like the get methods (doesn't remove the retured byte from the buffer)
	uint8_t peek(uint16_t index);

	int8_t isFull();
	//
	// Put methods, either a regular put in back or put in front
	//
	int8_t putInFront(uint8_t in);
	int8_t put(uint8_t in);
	int8_t puts(const __FlashStringHelper *ifsh);
	int8_t puts_P(const uint8_t *ifsh);
	int8_t puts(uint8_t *ifsh);
	//
	// Get methods, either a regular get from front or from back
	//
	uint8_t get();
	uint8_t getFromBack();
	//
	int8_t clearBeforAt(uint16_t index);
	int16_t strstr(int16_t start, const __FlashStringHelper *ifsh,size_t size);
	int16_t strstr(uint16_t start, uint8_t *ifsh,size_t size);
	int16_t strstr_P(uint16_t start, const uint8_t *ifsh,size_t size);
	void description();
	String getsToString(uint16_t len);
	int16_t gets(uint16_t len,uint8_t *buf,size_t size);
	void storeState();
	void restoreState();
private:
	uint8_t* data;

	uint16_t capacity;
	uint16_t position;
	uint16_t length;
	uint16_t stackPosition;
	uint16_t stackLength;
	uint16_t lengthToPosition(uint16_t pos);
};

#endif

