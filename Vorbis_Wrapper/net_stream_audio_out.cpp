#include <vogg/net_stream_audio_out.h>

namespace vogg
{
	NetStreamAudioOut::NetStreamAudioOut(wsock::udpSocket& local_addr, wsock::addr remote_addr)
													  : local(local_addr),remote(remote_addr)
	{
		pack.id = 1;
		pack.timestamp = 0;
		
	}

	NetStreamAudioOut::~NetStreamAudioOut() {}

	size_t NetStreamAudioOut::stream_read(void* tobuffer, size_t buffersize, size_t readamount)
	{
		return 0;
	}

	size_t NetStreamAudioOut::stream_write(const void* frombuffer, size_t writesize)
	{
		/* need seriliazation before sending */

		pack.id = pack.id++;
		pack.size = writesize;

		/* microsec */
		pack.timestamp = (uint32_t)(std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000); 
		memcpy(pack.data, frombuffer, writesize);

		return local._sendto(remote, &pack, sizeof(pack), 0);

		
	}
}