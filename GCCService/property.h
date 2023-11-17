
#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>

#include "rttr/registration.h"

#pragma comment(lib, "librttr_core.lib")

namespace RBX
{
	enum Types
	{
		Data,
		Behavior,
		Appearance,
		Game,
		Part
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
		}
		return "";
	}
}

#endif