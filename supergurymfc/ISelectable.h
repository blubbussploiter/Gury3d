#pragma once

#include <G3DAll.h>

namespace RBX
{
	class ISelectable
	{
	public:
		class SelectableBox
		{
		public:
			CoordinateFrame cframe;
			Vector3 size;
			SelectableBox() {}
			SelectableBox(CoordinateFrame cframe, Vector3 size) : cframe(cframe), size(size) {}
		};
	public:
		virtual SelectableBox getBoundingBox() { return SelectableBox(); };
	};
}