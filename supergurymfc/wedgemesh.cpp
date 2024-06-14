#include "mesh.h"
#include "part.h"

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

void RBX::Render::WedgeMesh::renderWedgeFace(NormalId face)
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