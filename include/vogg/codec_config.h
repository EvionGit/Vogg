#ifndef CODEC_CONFIG_HEADER_H
#define CODEC_CONFIG_HEADER_H

#include "base_definition.h"

namespace vogg
{
	struct CodecConfig
	{
		uint32_t channels;
		uint32_t rate;
		uint32_t max_bitrate;
		uint32_t nominal_bitrate;
		uint32_t min_bitrate;
		uint32_t bytes_per_sample;
		

		CodecConfig(uint32_t channels	      = 2,
					uint32_t rate		      = 44100,
					uint32_t bytes_per_sample = 2,
					uint32_t max_bitrate      = 66150,
					uint32_t min_bitrate      = -1,
					uint32_t nominal_bitrate  = -1);
					
					
	};
}

#endif