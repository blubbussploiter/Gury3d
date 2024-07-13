#include "renderMesh.h"

RBX::Render::Mesh* mesh;

void RBX::Render::Mesh::write(Vector3 vertex, Vector3 normal, Vector2 texCoord, Color4 color)
{
	worldGeometry_index.append(worldGeometry_index.size());
	worldGeometry_Vertexarray.append(vertex);
	worldGeometry_Normalarray.append(normal);
	worldGeometry_Texcoordarray.append(texCoord);
}

RBX::Render::Mesh* RBX::Render::Mesh::get()
{
	if (!mesh)
	{
		mesh = new RBX::Render::Mesh();
	}
	return mesh;
}