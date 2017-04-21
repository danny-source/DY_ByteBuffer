/*
Arduino Byte circle buffer
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DYByteBuffer.h"

#define DYBYTEBUFFER_DEBUG_MODE
#define DYBYTEBUFFER_DEBUG_PORT Serial
#ifdef DYBYTEBUFFER_DEBUG_MODE
	#define DYBYTEBUFFER_DEBUG_PRINTLN(...)  DYBYTEBUFFER_DEBUG_PORT.println (__VA_ARGS__)
	#define DYBYTEBUFFER_DEBUG_PRINT(...)  DYBYTEBUFFER_DEBUG_PORT.print (__VA_ARGS__)
	#define DYBYTEBUFFER_DEBUG_WRITE(x)  DYBYTEBUFFER_DEBUG_PORT.write (x)
#else
	#define DYBYTEBUFFER_DEBUG_PRINTLN(...) {}
	#define DYBYTEBUFFER_DEBUG_PRINT(...)	{}
	#define DYBYTEBUFFER_DEBUG_WRITE(x)		{}
#endif
DYByteBuffer::DYByteBuffer(){

}

void DYByteBuffer::init(uint16_t size){
	data = (uint8_t*)malloc(sizeof(uint8_t)*size);
	capacity = size;
	position = 0;
	length = 0;
}

void DYByteBuffer::init(uint8_t *buf,uint16_t size){
	data = buf;
	capacity = size;
	position = 0;
	length = 0;
}

void DYByteBuffer::deAllocate(){
	free(data);
}

void DYByteBuffer::clear(){
	position = 0;
	length = 0;
}

int8_t DYByteBuffer::clearBeforAt(uint16_t index) {
	if (index > length) return 0;
	position = (position + index) % capacity;
	length = length - index;
	return 1;
}

uint16_t DYByteBuffer::getSize(){
	return length;
}

uint16_t DYByteBuffer::getCapacity(){
	return capacity;
}

uint8_t DYByteBuffer::peek(uint16_t index){
	uint8_t b = data[(position + index) % capacity];
	return b;
}

int8_t DYByteBuffer::isFull() {
	if(length < capacity){
		return 0;
	}
	return 1;
}

int8_t DYByteBuffer::put(uint8_t in){
	if(length < capacity){
		// save data byte at end of buffer
		data[(position + length) % capacity] = in;
		// increment the length
		length++;
		return 1;
	}
	// return failure
	return 0;
}

void DYByteBuffer::storeState(){
	stackPosition = position;
	stackLength = length;
}
void DYByteBuffer::restoreState(){
	position = stackPosition;
	length = stackLength;
}

int8_t DYByteBuffer::puts(const __FlashStringHelper *ifsh) {
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  while (1) {
    uint8_t c = (uint8_t)pgm_read_byte(p++);
    if (c == 0) break;
    if(length >= capacity){
		return 0;
	}
    if (put(c) == 0) {
		return 0;
	}
  }
  return 1;;
}

int8_t DYByteBuffer::puts_P(const uint8_t *ifsh) {
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  while (1) {
    uint8_t c = (uint8_t)pgm_read_byte(p++);
    if (c == 0) break;
    if(length >= capacity){
		return 0;
	}
    if (put(c) == 0) {
		return 0;
	}
  }
  return 1;
}

int8_t DYByteBuffer::puts(uint8_t *ifsh) {
  uint8_t *p = ifsh;
  while (1) {
    uint8_t c = (uint8_t)*(p++);
    if (c == 0) break;
    if(length >= capacity){
		return 0;
	}
    if (put(c) == 0) {
		return 0;
	}
  }
  return 1;
}

int8_t DYByteBuffer::putInFront(uint8_t in){
	if(length < capacity){
		// save data byte at end of buffer
		if( position == 0 ) {
			position = capacity-1;
		}else {
			position = (position- 1) % capacity;
		}
		data[position] = in;
		// increment the length
		length++;
		return 1;
	}
	// return failure
	return 0;
}

uint8_t DYByteBuffer::get(){
	uint8_t b = 0;
	if(length > 0){
		b = data[position];
		// move index down and decrement length
		position = (position + 1) % capacity;
		length--;
	}

	return b;
}

uint8_t DYByteBuffer::getFromBack(){
	uint8_t b = 0;
	if(length > 0){
		b = data[(position+length - 1) % capacity];
		length--;
	}

	return b;
}

//find & search
uint16_t DYByteBuffer::lengthToPosition(uint16_t pos){
	return (position + pos) % capacity;
}

void DYByteBuffer::description() {
	uint16_t len = getSize();
	for (uint16_t i = 0;i < len; i++) {
		DYBYTEBUFFER_DEBUG_WRITE(data[((position + i) % capacity)]);
	}
	DYBYTEBUFFER_DEBUG_PRINTLN("");
}

int16_t DYByteBuffer::strstr(int16_t start, const __FlashStringHelper *ifsh,size_t size) {
	if (getSize() < size) return -1;
	return strstr_P(start,(const uint8_t *)ifsh,size);
}

int16_t DYByteBuffer::strstr(uint16_t start, uint8_t *ifsh,size_t size) {
	if (getSize() < size) return -1;
	uint32_t location =0;
	uint32_t shift_location = (start <0?0:start);
	uint16_t result = 0;
	while ((shift_location + size) <= getSize()) {
	  uint8_t *p = ifsh;

	  while (1) {
		uint8_t c2 = (uint8_t)(*(p++));
		uint8_t c1 = (uint8_t)peek(location + shift_location);
		result = c1 - c2;
		if (result!=0 || c2 == '\0'| (location + shift_location) == getSize()) {
			break;
		}
		location++;
		if (location == size) {
			return shift_location;
		}
	  }
		shift_location++;
	}
  return -1;
}

int16_t DYByteBuffer::strstr_P(uint16_t start, const uint8_t *ifsh,size_t size) {
	uint32_t location =0;
	uint32_t shift_location = (start <0?0:start);
	uint8_t result = 0;
	while ((shift_location+size) <= getSize()) {
	  PGM_P p = reinterpret_cast<PGM_P>(ifsh);

	  while (1) {
		uint8_t c2 = (uint8_t)pgm_read_byte(p++);
		uint8_t c1 = (uint8_t)peek(location + shift_location);
		result = c1 - c2;
		if (result!=0 || c2 == '\0'| (location + shift_location) == getSize()) {
			break;
		}
		location++;
		if (location == size) {
			return shift_location;
		}
	  }
		shift_location++;
	}
  return -1;
}

String DYByteBuffer::getsToString(uint16_t len) {
	String str;
	for (int16_t i =0;i < len;i++) {
		uint8_t b = get();
		if (b == 0) break;
		str = str + b;
	}
	return str;
}

int16_t DYByteBuffer::gets(uint16_t len,uint8_t *buf,size_t size) {
	for (int16_t i =0;i < len;i++) {
		uint8_t b = get();
		if (b == 0) return 1;
		if (i >= size) return 0;
		buf[i] = b;
	}
	return 1;
}
