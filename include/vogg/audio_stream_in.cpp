
#include "vogg/audio_stream_in.h"

namespace vogg
{
	AudioStreamIn::AudioStreamIn() : s(0),e(0),size(0) {}

	AudioStreamIn::~AudioStreamIn(){}

	size_t AudioStreamIn::stream_read(void* tobuffer, size_t buffersize, size_t readamount)
	{
		while(1)
		{
			mtx.lock();
			if (!size)
			{
				mtx.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
			else
			{
				int toread = size > readamount ? readamount : size;
				memcpy(tobuffer, audiobuffer + s, toread);
				s += toread;
				printf("data read: %i\n",toread);
				size -= toread;
				if (!size)
				{
					e = 0;
					s = 0;
				}
				mtx.unlock();
				return toread;
			}
		}	

		
		
	}

	size_t AudioStreamIn::stream_write(const void* frombuffer, size_t writesize)
	{
		std::lock_guard<std::mutex> lock(mtx);
		printf("data write: %zu\n", writesize);

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

			if (s <= e) s = e;

			size = AUDIOBUFF - s - e;

			
		}
		
		

		return writesize;

	}
}