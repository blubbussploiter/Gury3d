
#ifndef MESH_H
#define MESH_H

#include "pvenums.h"
#include "pvinstance.h"

#include "content.h"
#include "stdout.h"

namespace RBX
{
	namespace Render
	{
		extern void buildHeadMesh(Vector3 size);

		enum MeshType
		{
			Head,
			Torso,
			Wedge,
			Sphere,
			Cylinder,
			Filemesh,
			Brick
		};

		class SpecialMesh : 
			public RBX::Render::Renderable
		{
		private:
			Content meshId;
			std::vector<Vector3> vertices;
			std::vector<Vector3> normals;
			std::vector<Vector3> uvs;
			Vector3 mesh_scale;
			MeshType meshType;
			int faces;
			int num_faces;
		public:

			MeshType getMeshType() { return meshType; }

			void setMeshType(MeshType meshType)
			{
				switch (meshType)
				{
					case Head:
					{
						fromFile(GetFileInPath("/content/font/head.mesh"));
						break;
					}
					default: break;
				}
			}

			void setMeshScale(Vector3 scale)
			{
				mesh_scale = scale;
			}

			Vector3 getMeshScale() { return mesh_scale; }

			void fromFile(std::string path);
			void fromMeshType(MeshType types);

			void setMeshId(Content meshId);
			Content getMeshId() { return meshId; }

			void renderDecals(RenderDevice* d);
			void render(RenderDevice* d);

			SpecialMesh()
			{
				setClassName("SpecialMesh");
				setName("SpecialMesh");
				setMeshType(Head);
				mesh_scale = Vector3::one();
				isSpecialShape = true;
			}
			virtual ~SpecialMesh() {}

			RTTR_ENABLE(RBX::Render::Renderable)
		};
	}
}

#endif