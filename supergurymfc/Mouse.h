#pragma once

#include <vector>
#include <G3DAll.h>

#include "appmanager.h"

#include "instance.h"
#include "pvinstance.h"
#include "camera.h"
#include "ray.h"

namespace RBX
{
	class Mouse
	{
	public:

		bool hoveringUI;
		float cx, cy, x, y;

		Vector3 hitWorld, dir;

		RBX::PVInstance* target;

		RBX::PVInstance* getTarget();

		Vector3 getDir() { getTarget(); return dir; }
		Vector3 getHit() { getTarget(); return hitWorld; }

		void update(UserInput* ui);
		void render(RenderDevice* rd);

		template<typename IgnoredItem>
		inline RBX::PVInstance* getTargetWithIgnoreList(std::vector<IgnoredItem*>& ignore)
		{
			Ray ray;
			Rect2D viewport;

			viewport = AppManager::singleton()->getApplication()->getViewport();
			Camera* camera = Camera::singleton();

			ray = camera->camera->worldRay(x, y, viewport);
			target = (PVInstance*)World::getPartFromG3DRay<IgnoredItem>(ray, hitWorld, 0, ignore);

			return target;
		}

		static Mouse* getMouse();

		Mouse()
		{

		}
	};

}