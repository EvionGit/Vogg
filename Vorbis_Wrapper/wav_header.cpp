#include "vogg/wav_header.h"

namespace vogg
{
    WaveHeader::WaveHeader(uint32_t sample_rate, uint16_t bits_per_sample, uint16_t channels, uint32_t data_size)
    {
        chunkSize += data_size;
        sampleRate = sample_rate;
        bitsPerSample = bits_per_sample;
        numChannels = channels;
        subchunk2Size = data_size;

    }
}