#include "mesh.h"
#include "lighting.h"

#include "part.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::Render::SpecialMesh>("SpecialMesh")
		.constructor<>()
		.property("Scale", &RBX::Render::SpecialMesh::getMeshScale, &RBX::Render::SpecialMesh::setMeshScale)
		.property("MeshId", &RBX::Render::SpecialMesh::getMeshId, &RBX::Render::SpecialMesh::setMeshId)
		.property("MeshType", &RBX::Render::SpecialMesh::getMeshType, &RBX::Render::SpecialMesh::setMeshType)(rttr::metadata("Type", RBX::Data));
	rttr::registration::enumeration<RBX::Render::MeshType>("MeshType")
		(
			rttr::value("Head", RBX::Render::Head),
			rttr::value("Torso", RBX::Render::Torso),
			rttr::value("Wedge", RBX::Render::Wedge),
			rttr::value("Sphere", RBX::Render::Sphere),
			rttr::value("Cylinder", RBX::Render::Cylinder),
			rttr::value("FileMesh", RBX::Render::Filemesh),
			rttr::value("Brick", RBX::Render::Brick)
		);

}

static void drawFace(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
	glBegin(GL_QUADS);
	glNormal((v0 - v1).cross(v0 - v2).direction());

	glTexCoord2d(uv.x, uv.y);
	glVertex(v0);
	glTexCoord2d(0, uv.y);
	glVertex(v1);
	glTexCoord2d(0, 0);
	glVertex(v2);
	glTexCoord2d(uv.x, 0);
	glVertex(v3);
	glEnd();
}

void drawTriangleFace(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2)
{
	glBegin(GL_TRIANGLES);

	glNormal((v0 - v1).cross(v0 - v2).direction());

	glTexCoord2d(uv.x, uv.y);
	glVertex(v0);
	glTexCoord2d(0, uv.y);
	glVertex(v1);
	glTexCoord2d(0, 0);
	glVertex(v2);

	glEnd();
}

void RBX::Render::SpecialMesh::renderWedgeFace(NormalId face)
{

	PartInstance* part = toInstance<PartInstance>(getParent());
	if (!part) return;


	Vector3 size = part->getSize();
	Vector2 uv0(size.x, -size.z / 2), uv1, uv2;

	switch (face)
	{
	case BOTTOM:
	{
		drawFace(uv0,
			Vector3(size.x, -size.y, size.z),
			Vector3(-size.x, -size.y, size.z),
			Vector3(-size.x, -size.y, -size.z),
			Vector3(size.x, -size.y, -size.z));
		break;
	}
	case TOP: break;
	case FRONT:
	{
		drawFace(uv1,
			Vector3(-size.x, -size.y, -size.z),
			Vector3(-size.x, size.y, size.z),
			Vector3(size.x, size.y, size.z),
			Vector3(size.x, -size.y, -size.z));
		break;
	}
	case BACK:
	{
		drawFace(uv1,
			Vector3(size.x, size.y, size.z),
			Vector3(-size.x, size.y, size.z),
			Vector3(-size.x, -size.y, size.z),
			Vector3(size.x, -size.y, size.z));
		break;
	}
	case LEFT:
	{
		drawTriangleFace(uv2,
			Vector3(size.x, -size.y, size.z),
			Vector3(size.x, -size.y, -size.z),
			Vector3(size.x, size.y, size.z));
		break;
	}
	case RIGHT:
	{
		drawTriangleFace(uv2, 
			Vector3(-size.x, -size.y, -size.z),
			Vector3(-size.x, -size.y, size.z),
			Vector3(-size.x, size.y, size.z));
		break;
	}
	}
}

void RBX::Render::SpecialMesh::renderSpecialMesh(RenderDevice* d)
{

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < num_faces; ++i)
	{
		glNormal(normals[i] * mesh_scale);
		glTexCoord(uvs[i]);
		glVertex(vertices[i] * mesh_scale);
	}

	glEnd();

}

void RBX::Render::SpecialMesh::fromMeshType(MeshType types)
{
	setMeshType(types);
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
	meshId = Content::fromContent(path);
}

void RBX::Render::SpecialMesh::setMeshId(Content SpecialMeshId)
{
	std::string contentPath;

	if (meshType != Filemesh)
	{
		return;
	}

	/* something... */

	vertices.clear();
	normals.clear();
	uvs.clear();

	fromFile(contentPath);
}

void RBX::Render::SpecialMesh::renderFaceFitForDecal(RenderDevice* rd, NormalId face)
{
	renderFace(rd, face);
}

void RBX::Render::SpecialMesh::renderFace(RenderDevice* d, NormalId face)
{
	switch (meshType)
	{
		case Head:
		case Filemesh:
		{
			renderSpecialMesh(d);
			break;
		}
		case Wedge:
		{
			renderWedgeFace(face);
			break;
		}
	}
}

void RBX::Render::SpecialMesh::renderDecal(RenderDevice* rd, Decal* decal)
{
	PartInstance* part = toInstance<PartInstance>(getParent());
	if (!part) return;

	if (!part->transparency)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	decal->render(rd, this);

	if (!part->transparency)
	{
		glBlendFunc(GL_ZERO, GL_ZERO);
		glDisable(GL_BLEND);
	}

}

void RBX::Render::SpecialMesh::render(RenderDevice* d)
{
	PartInstance* part = toInstance<PartInstance>(getParent());
	if (!part) return;

	d->setObjectToWorldMatrix(part->pv->position);
	d->setShininess(25.0f);

	d->setColor(Color4(part->color, part->alpha));

	switch (meshType)
		{
		case Wedge:
		{
			renderFace(d, LEFT);
			renderFace(d, RIGHT);
			renderFace(d, BACK);
			renderFace(d, FRONT);
			renderFace(d, BOTTOM);
			renderFace(d, TOP);
			break;
		}
		default:
		{
			renderFace(d, UNDEFINED);
			break;
		}
	}
}

void RBX::Render::buildHeadMesh(Vector3 size)
{

}
