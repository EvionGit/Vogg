#ifndef VORBIS_CODEC_HEADER_H
#define VORBIS_CODEC_HEADER_H

#include "base_definition.h"
#include <vorbisenc.h>


namespace vogg
{
	struct Vorbis 
	{
		vorbis_info vinfo;
		vorbis_dsp_state vdsp;
		vorbis_comment vcomment;
		vorbis_block vblock;

	};
}

#endif