#pragma once

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "instance.h"

namespace RBX
{
	class Serializer
	{
	private:
		static bool checkTag();
	public:
		static void load(std::string fileName);
		static RBX::Instances* loadInstances(std::string fileName);
	};
}

#endif