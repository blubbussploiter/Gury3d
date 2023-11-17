#include "mesh.h"
#include "lighting.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::Render::SpecialMesh>("SpecialMesh")
		.constructor<>()
		.property("Scale", &RBX::Render::SpecialMesh::getMeshScale, &RBX::Render::SpecialMesh::setMeshScale)
		.property("MeshId", &RBX::Render::SpecialMesh::getMeshId, &RBX::Render::SpecialMesh::setMeshId)
		.property("MeshType", &RBX::Render::SpecialMesh::getMeshType, &RBX::Render::SpecialMesh::setMeshType);
}

void RBX::Render::SpecialMesh::fromFile(std::string path)
{
	FILE* f = fopen(path.c_str(), "r");

	float vx, vy, vz,
		nx, ny, nz,
		ux, uy, uz;

	fscanf(f, "version 1.00\n");
	fscanf(f, "%d\n", &faces);

	for (int i = 0; i < faces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fscanf(f, "[%f,%f,%f][%f,%f,%f][%f,%f,%f]", &vx, &vy, &vz, &nx, &ny, &nz, &ux, &uy, &uz);
			vx *= 0.5;
			vy *= 0.5;
			vz *= 0.5;
			uy = 1.0 - uy;
			vertices.push_back(Vector3(vx, vy, vz));
			normals.push_back(Vector3(nx, ny, nz));
			uvs.push_back(Vector3(ux, uy, uz));
		}
	}

	fclose(f);

	num_faces = faces * 3;
	meshId = Content(path);
}

void RBX::Render::SpecialMesh::fromMeshType(MeshType types)
{
	setMeshType(types);
}

void RBX::Render::SpecialMesh::setMeshId(Content SpecialMeshId)
{
	std::string contentPath;

	RBX::ContentProvider::singleton()->downloadContent(SpecialMeshId, contentPath);

	if (contentPath.empty()) return;

	vertices.clear();
	normals.clear();
	uvs.clear();

	fromFile(contentPath);

	RBX::ContentProvider::singleton()->cleanupContent(SpecialMeshId);
}

void RBX::Render::SpecialMesh::renderDecals(RenderDevice* rd)
{
	RBX::Instances* children;
	children = getParent()->getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Decal* d = dynamic_cast<RBX::Decal*>(children->at(i));
		if (d)
		{
			d->render(rd, this);
		}
	}
}

void RBX::Render::SpecialMesh::render(RenderDevice* d)
{
	switch (meshType)
	{
	case Head:
	{
		buildHeadMesh(mesh_scale);
		break;
	}
	default: break;
	}
}

void RBX::Render::SpecialMesh::renderFace(RBX::NormalId face, bool isDrawingDecal)
{
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < num_faces; ++i)
	{
		glNormal(normals[i]);
		glTexCoord(uvs[i]);
		glVertex(vertices[i]);
	}

	glEnd();
}

void RBX::Render::buildHeadMesh(Vector3 size)
{

}
