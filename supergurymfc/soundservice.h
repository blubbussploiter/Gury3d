#pragma once

#include "sounds.h"
#include "service.h"

namespace RBX
{
	class SoundService :
		public RBX::Instance, public RBX::Service<SoundService>
	{
	public:
		FMOD::System* mpSystem;

		void init();
		void update();

		static SoundService* get();

		SoundService()
		{
			setName("SoundService");
			setClassName("SoundService");
		}
	};

}