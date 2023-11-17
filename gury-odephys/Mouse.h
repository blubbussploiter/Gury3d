#ifndef MOUSE_H
#define MOUSE_H

#include <G3DAll.h>
#include "pvinstance.h"

namespace Rendering
{
	extern TextureRef cursor_custom;
	extern unsigned int mouse_glid;
	extern bool isOverGuiObject;
	extern bool shouldRenderAsFar();
}

namespace RBX
{
	class Mouse
	{
	private:
		static Vector3 hitWorld, dir;
	public:

		static RBX::PVInstance* target;
		static RBX::PVInstance* getTarget(RBX::PVInstance* ignorePart=0);

		static Vector3 getDir() { getTarget(); return dir; }
		static Vector3 getHit() { getTarget(); return hitWorld; }

		static void update(UserInput* ui);
		static void render(RenderDevice* rd);
	};
}
#endif