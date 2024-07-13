
#include "soundservice.h"

RBX::SoundService* soundService;

void RBX::SoundService::update()
{
	mpSystem->update();
}

void RBX::SoundService::init()
{
	FMOD::System_Create(&mpSystem);
	mpSystem->init(32, FMOD_INIT_NORMAL, 0);
}

RBX::SoundService* RBX::SoundService::get()
{
	if (!soundService)
	{
		soundService = new SoundService();
		soundService->init();
	}
	return soundService;
}