#ifndef SCENE_H
#define SCENE_H

#include "IRenderable.h"
#include "ISteppable.h"

#include "runservice.h"

namespace RBX
{

	typedef std::vector<RBX::Render::Renderable*> IRenderableArray;

	class Scene : public RBX::Instance
	{
	public:

		IRenderableArray renderObjects;

		void onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent);
		void onWorkspaceDescendentRemoved(RBX::Render::Renderable* descendent);

		void updateSteppables();

		void iterate(RenderDevice* rd, bool(*rule)(RBX::Render::Renderable*), void(*render)(RBX::Render::Renderable*, RenderDevice*));

		void opaquePass(RenderDevice* rd);
		void reflectancePass(RenderDevice* rd);
		void transparentPass(RenderDevice* rd);
		void darkPass(RenderDevice* rd);

		void lastPass(RenderDevice* rd);
		
		void initializeKernel();
		void saveStartPVs();

		static bool isRenderable(RBX::Instance* instance) { return dynamic_cast<RBX::Render::Renderable*>(instance) != 0; }

		static Scene* singleton();
		void close();

		Scene()
		{

		}

		std::vector<RBX::Render::Renderable*> getArrayOfObjects();
	};
}

#endif