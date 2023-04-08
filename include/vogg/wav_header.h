#ifndef WAV_HEAD_HEADER_H
#define WAV_HEAD_HEADER_H

#include "base_definition.h"

namespace vogg
{
    struct WaveHeader
    {
        char chunkId[4] = { 'R','I','F','F' };
        uint32_t chunkSize = 44;
        char format[4] = { 'W','A','V','E' };
        char subchunk1Id[4] = { 'f','m','t',' ' };
        uint32_t subchunk1Size = 16;
        uint16_t audioFormat = 1;
        uint16_t numChannels = 2;
        uint32_t sampleRate = 44100;
        uint32_t byteRate = subchunk1Size * numChannels * sampleRate / 8;
        uint16_t blockAlign = subchunk1Size * numChannels / 8;
        uint16_t bitsPerSample = 16;
        char subchunk2Id[4] = { 'd','a','t','a' };
        uint32_t subchunk2Size = 0;


        WaveHeader(uint32_t sample_rate, uint16_t bits_per_sample, uint16_t channels, uint32_t data_size);
    };
}

#endif