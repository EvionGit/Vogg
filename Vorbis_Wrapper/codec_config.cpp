#include "vogg/base_definition.h"
#include "vogg/codec_config.h"

namespace vogg
{
	CodecConfig::CodecConfig(uint32_t channels,
							 uint32_t rate,
							 uint32_t bytes_per_sample,
							 uint32_t max_bitrate,
							 uint32_t min_bitrate,
							 uint32_t nominal_bitrate)
	{
		this->channels = channels;
		this->rate = rate;
		this->bytes_per_sample = bytes_per_sample;
		this->max_bitrate = max_bitrate;
		this->min_bitrate = min_bitrate;
		this->nominal_bitrate = nominal_bitrate;
	}

}