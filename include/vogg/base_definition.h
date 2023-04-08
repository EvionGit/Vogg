#ifndef BASE_VORBIS_HEADER_H
#define BASE_VORBIS_HEADER_H

#include <stdio.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <stdint.h>
#include <exception>
#include "stream.h"


#define SAMPLES_SIZE 4096 // DECODING: size of samples buffer for decoding
#define WRITE_CHUNK 800 // ENCODING: approximate page size that will be sent to OUT-stream

static int last_error = 0;

#endif