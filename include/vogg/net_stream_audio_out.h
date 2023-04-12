#ifndef NET_STREAM_AUDIO_OUT_HEADER
#define NET_STREAM_AUDIO_OUT_HEADER

#include "stream.h"
#include <wsock/udp_socket.h>
#include <wsock/audio_packet.h>

namespace vogg
{
	class NetStreamAudioOut : public Stream
	{
	private:
		wsock::AudioPacket pack;
		wsock::udpSocket& local;
		wsock::addr& remote;
	public:
		NetStreamAudioOut(wsock::udpSocket& local_addr, wsock::addr remote_addr);
		~NetStreamAudioOut();
	
	public:
		virtual size_t stream_write(const void* frombuffer, size_t writesize) override;

	private:
		virtual size_t stream_read(void* tobuffer, size_t buffersize, size_t readamount) override;
	};
}

#endif