#ifndef VOGG_DECODER_HEADER_H
#define VOGG_DECODER_HEADER_H

#include "base_definition.h"
#include "ogg_container.h"
#include "vorbis_codec.h"
#include "codec_config.h"
#include "wav_header.h"

namespace vogg
{
	class Decoder : public Ogg, public Vorbis
	{
	private:
		CodecConfig in_conf;
		CodecConfig out_conf;
		Stream& in;
		Stream& out;
		size_t decoded_size;
		int16_t samplesbuffer[SAMPLES_SIZE];
		

	public:
		Decoder(Stream& in, Stream& out, CodecConfig in_conf, CodecConfig out_conf);
		~Decoder();

	public:
		/* decode from OGG/VORBIS to PCM */
		void decode();

		/* return last decoded data size after EOF */
		size_t get_decoded_size();

	private:
		/* init base structures */
		void init();

		/* init stream with page serial num */
		void init_stream();

		/* setup received codec headers from IN-STREAM */
		void receive_headers();

		/* get page from IN-STREAM */
		int receive_page();

	};
}

#endif
