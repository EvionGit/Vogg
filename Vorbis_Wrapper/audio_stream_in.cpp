
#include "vogg/audio_stream_in.h"

namespace vogg
{
	AudioStreamIn::AudioStreamIn() : s(0),e(0),size(0)
	{
		memset(audiobuffer, 0, AUDIOBUFF);
	}

	AudioStreamIn::~AudioStreamIn(){}

	size_t AudioStreamIn::stream_read(void* tobuffer, size_t buffersize, size_t readamount)
	{
		while(1)
		{
			mtx.lock();
			if (size < 4096)
			{
				mtx.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				continue;
			}
			else if (AUDIOBUFF - s >= readamount)
			{
				
				memcpy(tobuffer, audiobuffer + s, readamount);
				s += readamount;
				size -= readamount;
				
			}
			else
			{
				int toread = AUDIOBUFF - s;
				memcpy(tobuffer, audiobuffer + s, toread);
				memcpy(tobuffer, audiobuffer, readamount - toread);

				s = readamount - toread;
				size -= readamount;
			
			}

			if (!size)
			{
				e = 0;
				s = 0;
			}
			mtx.unlock();
			return readamount;
		}	

		
		
	}

	size_t AudioStreamIn::stream_write(const void* frombuffer, size_t writesize)
	{
		std::lock_guard<std::mutex> lock(mtx);
		
	

		if ((AUDIOBUFF - e) >= writesize)
		{
			memcpy(audiobuffer + e, frombuffer, writesize);
			e += writesize;
			size += writesize;
			
		}
		else
		{
			size_t length = AUDIOBUFF - e;

			memcpy(audiobuffer + e, frombuffer, length);
			memcpy(audiobuffer, (char*)frombuffer + length, writesize - length);

			e = writesize - length;
			if (s < e) s = e;

			size += writesize;

		}
		
		return writesize;

	}
}