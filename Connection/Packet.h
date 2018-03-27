#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
enum DataType{INT = 1, FLOAT = 2, LONG = 3, ULONG = 4, STRING = 5};

class Packet{
	public:
		Packet(void*, DataType, int, int);
        ~Packet();
		int size;
        int position;
		void * data;
		DataType dataType;
};

#endif
