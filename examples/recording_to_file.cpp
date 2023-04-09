
#include <SFML/Audio.hpp>
#include <vogg/audio_stream_in.h>
#include <vogg/filestream.h>

#include <vogg/encoder.h>

/* simply realization for recorder */
class AudioRecorder : public sf::SoundRecorder
{

private:
	vogg::AudioStreamIn& astream;

public:
	AudioRecorder(vogg::AudioStreamIn& astream) : astream(astream)
	{
		setProcessingInterval(sf::Time(sf::milliseconds(40)));
	}
	virtual  bool onProcessSamples(const sf::Int16* samples, size_t sampleCount) override
	{
		astream.stream_write(samples, sampleCount * 2);


		return 1;
	}
};


void recording_to_file()
{
	/* implemented streams for encoder.
	   
	   In this case: AudioRecorder record our voice
	   and move raw samples to the AudioStreamIn,
	   then encoder takes raw data, encode it to OGG
	   and move compressed audio to FileStream which saves it to file.
	*/

	vogg::AudioStreamIn ainput;
	vogg::FileStream aoutput("voice.ogg", "wb");

	vogg::CodecConfig c1(1), c2(2); // codec config for Input and Output stream (mono to stereo)

	AudioRecorder rec(ainput); // AudioRecorder implementation by SFML::SoundRecorder

	vogg::Encoder encoder(ainput, aoutput, c1, c2); // Encoder takes IN and OUT stream with them configs

	printf("recording...\n");
	rec.start(); // recording executing in other thread

	encoder.encode();

	/* in this version of code: encoder doesnt handle EOF and working infinite */
	/* CTRL + C to stop */
	rec.stop();
	printf("recorded.\n");
}