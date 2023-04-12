#include <vogg/net_stream_audio_in.h>

namespace vogg
{
	NetStreamAudioIn::NetStreamAudioIn() : cur(1), size(0), chunk_size(0)
	{
		memset(&abuf, 0, sizeof(abuf));
		memset(&part, 0, sizeof(part));
		
	}

	size_t NetStreamAudioIn::stream_read(void* tobuffer, size_t buffersize, size_t readamount) 
	{
		
		
		while (chunk_size < readamount) // need EOF
		{
			mtx.lock();
			if (!size)
			{
				mtx.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				continue;
			}
			
			int cur_pos = cur % 30;
			int data_need = readamount - chunk_size;

			if (abuf[cur_pos].id == 0)
			{
				if (part.id)
				{

					memcpy((char*)tobuffer + chunk_size, part.data, part.size);
					data_need -= part.size;
					chunk_size += part.size;

					part.id = 0;

					memset((char*)tobuffer + chunk_size, 0, data_need > sizeof(part.data) ? sizeof(part.data) : data_need);
				}

				
				
			}
			else
			{

				if (part.id)
				{
					memcpy((char*)tobuffer + chunk_size, part.data, part.size);
					data_need -= part.size;
					chunk_size += part.size;
					
					part.id = 0;
				}

				if(data_need >= abuf[cur_pos].size)
				{
					memcpy((char*)tobuffer + chunk_size, abuf[cur_pos].data, abuf[cur_pos].size);
					data_need -= abuf[cur_pos].size;
					chunk_size += abuf[cur_pos].size;

					abuf[cur_pos].id = 0;
					cur++;
					size--;
				}
				else if(data_need < abuf[cur_pos].size)
				{
					memcpy((char*)tobuffer + chunk_size, abuf[cur_pos].data, data_need);
					data_need -= data_need;
					chunk_size += data_need;
					
					part.id = 1;
					part.size = abuf[cur_pos].size - data_need;
					memcpy(part.data, abuf[cur_pos].data + data_need, part.size);
					
					abuf[cur_pos].id = 0;
					cur++;
					size--;
				}
				
			}
			mtx.unlock();


		}
		chunk_size = 0;
		return readamount;


		

	}
	size_t NetStreamAudioIn::stream_write(const void* frombuffer, size_t writesize)
	{
		wsock::AudioPacket*ap = (wsock::AudioPacket*)frombuffer;
		uint16_t pos = ap->id % 30;

		mtx.lock();

		if (abuf[pos].id != ap->id && cur <= pos)
		{
			if (size == 0)
				cur = pos;

			abuf[pos] = *ap;

			size++;

			mtx.unlock();
			return writesize;
		}

		mtx.unlock();
		return 0;
	}
}