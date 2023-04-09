#include "vogg/decoder.h"

namespace vogg
{
	Decoder::Decoder(Stream& in, Stream& out, CodecConfig in_conf, CodecConfig out_conf) : in(in),out(out)
	{
		this->in_conf = in_conf;
		this->out_conf = out_conf;
		init();
	}

	Decoder::~Decoder()
	{
		ogg_sync_clear(&osync);
		vorbis_info_clear(&vinfo);
		vorbis_comment_init(&vcomment);
	}

	void Decoder::init()
	{
		
		ogg_sync_init(&osync);
		vorbis_info_init(&vinfo);
		vorbis_comment_init(&vcomment);
	
	}

	void Decoder::decode()
	{
		/* get codec info headers from IN-STREAM */
		receive_headers();

		/* init dsp for decoding */
		vorbis_synthesis_init(&vdsp, &vinfo);

		/* init block for decoded packets */
		vorbis_block_init(&vdsp, &vblock);

		decoded_size = 0;
		/* while there are pages in IN-STREAM */
		while (receive_page())
		{
			/* add page to the stream */
			ogg_stream_pagein(&ostream, &opage);

			int status;

			/* get a packet from the stream */
			while ((status = ogg_stream_packetout(&ostream, &opacket)) == 1)
			{
				/* break packet to blocks */
				vorbis_synthesis(&vblock, &opacket);

				/* decode blocks with DSP codec parameters */
				vorbis_synthesis_blockin(&vdsp, &vblock);

				/* pointer to raw data samples */
				float** data;
				int samples;

				/* get buffer with decoded data */
				while ((samples = vorbis_synthesis_pcmout(&vdsp, &data)) > 0)
				{

					/* available samples count to write */
					int avail = samples > SAMPLES_SIZE ? SAMPLES_SIZE : samples;
					for (int i = 0; i < avail; i++)
					{

						/* cast data to 16-bit sample */
						int val = (int)floor(data[0][i] * 32767.f + .5f);

						if (val > 32767)
							val = 32767;

						if (val < -32768)
							val = -32768;

						/* copy thew to line buffer */
						samplesbuffer[i * 2] = (int16_t)val;
						samplesbuffer[i * 2 + 1] = (int16_t)val;
					}

					/* send samples buffer to OUT-STREAM */
					size_t w = out.stream_write(samplesbuffer, avail*4);
					
					/* accumulate all wrote samples in bytes*/
					decoded_size += w * 4;

					/* informs the decoder how many samples used from last buffer output */
					vorbis_synthesis_read(&vdsp, (int)(w/4));
				}

			}
			
		}

		/* clear dependent structures after EOF */
		vorbis_block_clear(&vblock);
		vorbis_dsp_clear(&vdsp);
		ogg_stream_clear(&ostream);
	}

	void Decoder::init_stream()
	{
	
		/* init stream with received serial number */
		if (ogg_stream_init(&ostream, ogg_page_serialno(&opage)))
		{
			printf("<OGG_STREAM_INIT_ERROR>");
			throw std::exception("<OGG_STREAM_INIT_ERROR>");
		}
	}

	int Decoder::receive_page()
	{
		/* pointer to the SYNC inside buffer */
		char* buffer;

		while (ogg_sync_pageout(&osync, &opage) != 1) // if non-zero read another bytes
		{
			/* ask the SYNC buffer for reading */
			buffer = ogg_sync_buffer(&osync, 4096);

			/* fill the buffer from IN-STREAM`s data */
			size_t r = in.stream_read(buffer, 4096, 4096);

			/* tell the SYNC how many bytes we have read*/
			ogg_sync_wrote(&osync, (int)r);

			if (r == 0) return 0;

		}
		return 1;
	}

	void Decoder::receive_headers()
	{
		if (!receive_page())
		{
			printf("<RECEIVE_HEADERS_ERROR> NO DATA RECEIVED");
			throw std::exception("<RECEIVE_HEADERS_ERROR>");
		}


		init_stream();
		ogg_stream_pagein(&ostream, &opage);

		int h = 0;
		int status, ecode;
		while (h < 3) // while dont read first 3 headers
		{
			
			if ((status = ogg_stream_packetout(&ostream, &opacket)) == 1)
			{
				if ((ecode = vorbis_synthesis_headerin(&vinfo, &vcomment, &opacket)) < 0) // set info
				{
					printf("<SYNTHESIS_HEADERIN_ERROR> CODE: %i",ecode);
					throw std::exception("<SYNTHESIS_HEADERIN_ERROR>");
				}
				h++;
			}
			else if (status == 0) // if insufficient data: add next page to stream
			{
				if (!receive_page())
				{
					printf("<RECEIVE_HEADERS_ERROR> NO DATA RECEIVED");
					throw std::exception("<RECEIVE_HEADERS_ERROR>");
				}

				ogg_stream_pagein(&ostream, &opage);

			}

		}
		
	}

	size_t Decoder::get_decoded_size()
	{
		return decoded_size;
	}
	

}