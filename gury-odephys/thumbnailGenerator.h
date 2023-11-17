#pragma once

#include <G3DAll.h>

#include "instance.h"
#include "oscontext.h"

#include "service.h"

namespace RBX
{
	class ThumbnailGenerator :
		public RBX::Instance,
		public Service<ThumbnailGenerator>
	{
	public:
		std::string click(std::string fileType, int width, int height, bool hideSky);
		void doClick(std::string fileType, int width, int height, bool imageServerView, BinaryOutput& out);
		ThumbnailGenerator()
		{
			setClassName("ThumbnailGenerator");
			setName("ThumbnailGenerator");
		}
	};
}