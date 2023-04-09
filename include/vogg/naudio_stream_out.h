#ifndef NET_AUDIO_STREAM_OUT_HEADER_H
#define NET_AUDIO_STREAM_OUT_HEADER_H

#include "stream.h"
#include "wsock/udp_socket.h"
#include "wsock/audio_packet.h"


#define RBUFF 30

namespace vogg
{
	class NetAudioStreamOut : public Stream
	{
	private:
		std::mutex mtx;
		wsock::udpSocket sock;
		wsock::addr remote;
		wpack::AudioPacket recvbuf[RBUFF];


	public:
		NetAudioStreamOut(wsock::udpSocket& sock, wsock::addr& remote);
		~NetAudioStreamOut();

	public:
		virtual size_t stream_read(void* tobuffer, size_t buffersize, size_t readamount) override;
		virtual size_t stream_write(const void* frombuffer, size_t writesize) override;

	public:
		int get_packet();

	};
}

#endif