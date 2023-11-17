#include "soundservice.h"

void RBX::Sound::stop()
{
	channel->stop();
}

void RBX::Sound::setStartPosition(double value)
{
	double oldPosition = getStartPosition();
	double soundLength = getLength();
	double newPosition = fmod(value, (soundLength ? soundLength : 1.0));
	if (oldPosition != newPosition)
	{
		startPosition = newPosition * 1000.0f;
	}
}

void RBX::Sound::play()
{
	if(!sound)
		SoundService::singleton()->mpSystem->createSound(soundPath.c_str(), 0, 0, &sound);

	sound->setMode(FMOD_LOOP_OFF);
	SoundService::singleton()->mpSystem->playSound(sound, 0, 0, &channel);
	channel->setPosition(startPosition, FMOD_TIMEUNIT_MS);
	channel->setVolume(volume);
}