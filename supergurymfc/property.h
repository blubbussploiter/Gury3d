#pragma once

#include <string>
#include <vector>

#include "rttr/registration.h"
#include "signal.h"

#pragma comment(lib, "librttr_core.lib")

namespace RBX
{
	enum Types
	{
		Data,
		Behavior,
		Appearance,
		Game,
		Part,
		Surface
	};

	static std::string getType(Types t)
	{
		switch (t)
		{
		case Data: return "Data";
		case Behavior: return "Behavior";
		case Appearance: return "Appearance";
		case Game: return "Game";
		case Part: return "Part";
		case Surface: return "Surface";
		}
		return "";
	}
}