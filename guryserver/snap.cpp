#include "snap.h"
#include "stdout.h"

RBX::SnapConnector* RBX::SnapConnector::getConnectingSnap(Primitive* prim)
{
	if (!prim->body) return 0;

	void* rawData = prim->body->getUserdata();
	if (!rawData) return 0;

	RBX::Instance* instance = static_cast<Instance*>(rawData);

	if (RBX::IsA<SnapConnector>(instance))
		return (SnapConnector*)instance;

	return 0;
}

void copyArray(G3D::Array<RBX::Primitive*>* array0, G3D::Array<RBX::Primitive*>* array1)
{
	(*array1) = (*array0); /* copy; */
	array0->fastClear(); /* clear! */
}

void RBX::SnapConnector::build()
{
	Vector3 offsetPosition;
	Matrix3 offsetRotation;

	if (!prim0->geom[0] || !prim1->geom[0]) return;

	body0 = prim0->body;
	body1 = prim1->body;

	SnapConnector* connector0, * connector1;
	connector0 = getConnectingSnap(prim0);
	connector1 = getConnectingSnap(prim1);

	if (connector0)
	{
		connector = connector0->connector;
		copyArray(connector0->primitives, primitives);
	}
	if (connector1 && !connector)
	{
		connector = connector1->connector;
		copyArray(connector1->primitives, primitives);
	}

	if (!connector)
	{
		connector = new Body();
		connector->createBody(Vector3(1, 1, 1));

		if (body0 && body1)
		{
			CoordinateFrame pos0, pos1, pos;
			Vector3 t0, t1;
			Matrix3 m0, m1;

			pos0 = prim0->pv->position;
			pos1 = prim1->pv->position;

			t0 = pos0.translation;
			t1 = pos1.translation;
			m0 = pos0.rotation;
			m1 = pos1.rotation;

			pos = (t0 + t1) / 2;
			pos.rotation = m0 * m1;

			body0->setDisabled(1);
			body1->setDisabled(1);
			
			connector->modifyPosition(pos);
		}
	}

	primitives->push_back(prim0);
	primitives->push_back(prim1);

	if (connector)
	{
		Vector3 cofm = connector->getPosition();
		Vector3 size, lastSize;

		for (int i = 0; i < primitives->size(); i++)
		{
			Primitive* prim = (*primitives)[i];

			if (prim0->body != 0 && prim1->body != 0)
			{
				connector->attachPrimitive(prim);
				prim->modifyOffsetWorldCoordinateFrame(prim->pv->position);
				connector->setDisabled(0);
			}
			else
			{
				prim->body = 0;
				connector->detachPrimitive(prim);
				connector->setDisabled(1);
			}

			size = prim->size;
			if (size.magnitude() > lastSize.magnitude())
			{
				const dReal* p = dGeomGetPosition(prim->geom[0]);
				cofm = Vector3(p[0], p[1], p[2]);
				lastSize = size;
			}

		}
		
		dMass mass;
		dBodyGetMass(connector->body, &mass);
		mass.c[0] = cofm.x;
		mass.c[1] = cofm.y;
		mass.c[2] = cofm.z;

		connector->modifyUserdata(this);
		connector->modifyMass(mass);
	}

}

void RBX::SnapConnector::unlink()
{
	if (!connector || !prim0->geom[0] || !prim1->geom[0]) return;

	connector->detachPrimitive(prim0);
	connector->detachPrimitive(prim1);
	connector->destroyBody();

	dGeomClearOffset(prim0->geom[0]);
	dGeomClearOffset(prim1->geom[0]);

	if (body0)
	{
		body0->setDisabled(0);
		body0->attachPrimitive(prim0);
	}

	if (body1)
	{
		body1->setDisabled(0);
		body1->attachPrimitive(prim1);
	}
}
