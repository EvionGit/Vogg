#ifndef AUDIO_PACKET_H
#define AUDIO_PACKET_H

#include <wsock/base_definition.h>


namespace wsock
{
	struct AudioPacket
	{
		uint16_t id;
		uint32_t timestamp;
		uint16_t size;
		char data[1200];

	};
}




#endif