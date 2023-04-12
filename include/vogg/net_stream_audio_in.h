#ifndef NET_STREAM_AUDIO_IN_HEADER_H
#define NET_STREAM_AUDIO_IN_HEADER_H

#include <vogg/stream.h>
#include <wsock/audio_packet.h>

#define APACKETBUF 30

namespace vogg
{
	
	class NetStreamAudioIn : public Stream
	{
	private:
		std::mutex mtx;
		wsock::AudioPacket abuf[APACKETBUF];
		wsock::AudioPacket part;
		char chunk[4096];
		int chunk_size;
		uint16_t cur;
		uint8_t size;

	public:
		NetStreamAudioIn();


	public:
		/* Decoder reads data from the buffer */
		virtual size_t stream_read(void* tobuffer, size_t buffersize, size_t readamount) override;

		/* Protocol writes data to the buffer */
		virtual size_t stream_write(const void* frombuffer, size_t writesize) override;

	};
}

#endif