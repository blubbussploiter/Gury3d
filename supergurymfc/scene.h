#ifndef SCENE_H
#define SCENE_H

#include "IRenderable.h"
#include "ISteppable.h"

#include "runservice.h"

namespace RBX
{

	class Scene : public RBX::Instance
	{
	public:

		Instances sceneObjects;

		void onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent);
		void onWorkspaceDescendentRemoved(RBX::Render::Renderable* descendent);

		void updateSteppables();
		void updateSteppablesKernelly();

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

		Instances getArrayOfObjects();
	};
}

#endif