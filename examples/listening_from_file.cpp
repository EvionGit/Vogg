#include <SFML/Audio.hpp>
#include <vogg/audio_stream_out.h>
#include <vogg/filestream.h>

#include <vogg/decoder.h>

/* simply realization for listener */
class AudioListener : public sf::SoundStream
{
private:

	vogg::AudioStreamOut& astream;
	char buff[4096]; // chunk for decoded data 

public:

	AudioListener(vogg::AudioStreamOut& astream) : astream(astream)
	{
		memset(buff, 0, 4096);
		initialize(2, 44100); // default init configs for listener (2 channels 44100hz)
	}

	/* overrided method which call every time when data need for execute */
	virtual bool onGetData(Chunk& data) override
	{
		int c = astream.stream_read(buff, 4096, 4096); // blocking func which waiting decoded data 
		if (!c)
			return false;

		data.samples = (sf::Int16*)buff;
		data.sampleCount = c / 2;

		return true;
	}

	virtual void onSeek(sf::Time timeOffset) override
	{
		// in this case we dont need seeking at audio stream
	}
};


void listen_from_file()
{
	/*
		Load file to FileStream from input file,
		decode comppressed data by Decoder and 
		move decoded samples to AudioStreamOut where
		samples are playing.
	*/

	vogg::FileStream ainput("epica.ogg", "rb"); // FileStream with .ogg sound file
	vogg::AudioStreamOut aoutput; // Stream with decoded audio buffer

	vogg::CodecConfig c1(2), c2(2); // configs for in and out streams (stereo to stereo)

	AudioListener lis(aoutput); // simply implemented from SFML class to listening audio data

	vogg::Decoder decoder(ainput, aoutput, c1, c2); // decoder from .ogg to PCM

	printf("playing...\n");
	lis.play(); // running in another thread

	decoder.decode();

	printf("stopped\n");
}