#ifndef SOUND_H
#define SOUND_H

#include "fmod.hpp"
#include "instance.h"

#include <vector>
#include <string>

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

namespace RBX
{
	class Sound : RBX::Instance
	{
	private:
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		bool b3d;
	public:
		bool isLooping;
		bool stream;

		float startPosition, length;
		float volume;

		std::string soundPath;

		void playOnce();
		void play();
		void stop();

		bool isPlaying()
		{ 
			bool b; 
			channel->isPlaying(&b);
			return b; 
		}
		double getStartPosition() { unsigned int position; channel->getPosition(&position, FMOD_TIMEUNIT_MS); return ((double)position) / 1000.0; }
		void setStartPosition(double value);
		double getLength() { unsigned int len; sound->getLength(&len, FMOD_TIMEUNIT_MS); return ((double)len) / 1000.0; }
		void setVolume(float vol) { volume = vol; }
		float getVolume() { return volume; }
		static RBX::Sound* fromFile(std::string file, bool isLooped = 0)
		{
			RBX::Sound* s = new RBX::Sound();
			s->soundPath = file;
			s->isLooping = isLooped;
			s->volume = 1.0f;
			return s;
		}
		virtual ~Sound() { sound->release(); }
	};

}

#endif
