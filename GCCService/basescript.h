#pragma once

#include "instance.h"

namespace RBX
{
	class BaseScript : public RBX::Instance
	{
	private:
		std::string source;
	public:

		void setSource(std::string newSource) { source = newSource; }
		std::string getSource() { return source; }

		BaseScript()
		{
			setClassName("BaseScript");
			setName("BaseScript");
		}
	};
}