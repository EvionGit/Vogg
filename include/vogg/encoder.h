#ifndef VOGG_ENCODER_HEADER_H
#define VOGG_ENCODER_HEADER_H

#include "base_definition.h"
#include "ogg_container.h"
#include "vorbis_codec.h"
#include "codec_config.h"

namespace vogg
{
	class Encoder : public Ogg, public Vorbis
	{
	private:
		CodecConfig in_conf;
		CodecConfig out_conf;
		Stream& in;
		Stream& out;
		char readbuffer[4096];
		char writebuffer[2048];

	public:
		Encoder(Stream& in, Stream& out, CodecConfig in_conf, CodecConfig out_conf);
		~Encoder();

	public:
		/* encode PCM data to OGG/VORBIS */
		void encode();

		void set_inconfig(CodecConfig newconf);

		void set_outconfig(CodecConfig newconf);

		/* send 3 packets with Vorbis codec headers */
		void send_headers();

	private:
		/* Base structures initialization */
		void init();

		/* get a page from stream and send to OUT */
		void push_page(int32_t page_size);

		/* send data from pages to OUT-STREAM */
		void send_page();

		/* audio channels converting */
		void stereo_x_stereo(float**& buf,size_t datasize);
		void stereo_x_mono(float**& buf, size_t datasize);
		void mono_x_stereo(float**& buf, size_t datasize);
		void mono_x_mono(float**& buf, size_t datasize);
	};
}

#endif