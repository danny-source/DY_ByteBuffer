
#include <DYByteBuffer.h>

DYByteBuffer dBuffer;
#define DSERIAL	Serial

void setup() {
	DSERIAL.begin(115200);
	dBuffer.init(50);
	DSERIAL.println();
}

void loop() {
	uint8_t a = '0';
	for (uint8_t i = 0; i <9;i++) {
		dBuffer.put(a);
		a++;
	}
	dBuffer.description();
	uint8_t b = dBuffer.get();
	b = dBuffer.get();
	dBuffer.description();
	delay(1000);
	dBuffer.clear();
}
