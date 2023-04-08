#include <vorbisfile.h>
#include <vorbisenc.h>
#include <stdio.h>
#include <cmath>
#include "vogg/filestream.h"
#include "vogg/encoder.h"
#include "vogg/decoder.h"

int main()
{
    
    
    vogg::FileStream in("epica1.wav", "wb");
    vogg::FileStream out("epica2.ogg", "rb");

    vogg::CodecConfig inc(1), onc;

    vogg::Decoder decoder(out, in, inc, onc);
    decoder.decode();

   /* vogg::Encoder encoder(out, in, inc, onc);
    
    encoder.send_headers();
    encoder.encode();*/

    vogg::WaveHeader w(44100, 16, 2, decoder.get_decoded_size());

    in.add_header(&w, sizeof(w));

    return 0;
}