#include "jointsservice.h"
#include "scene.h"

void RBX::JointsService::Experiment::getKernelWorldContacts()
{
    IRenderableArray instances = Scene::singleton()->getArrayOfObjects();\

	for (RBX::Instance* instance1 : instances)
	{
		for (RBX::Instance* instance2 : instances)
		{
			RBX::PVInstance* pvInstance1 = toInstance<PVInstance>(instance1);
			RBX::PVInstance* pvInstance2 = toInstance<PVInstance>(instance2);

			if (pvInstance1 == 0 || pvInstance2 == 0) continue;
			if (pvInstance1 == pvInstance2) continue;

			/* hacky lil' thing, might not work in the future */

			Vector3 modifiedSize1 = pvInstance1->getSize() * 1.1f;
			Vector3 modifiedSize2 = pvInstance2->getSize() * 1.1f;
			Box modifiedBox1 = pvInstance1->getCFrame().toWorldSpace(Box(-modifiedSize1, modifiedSize1));
			Box modifiedBox2 = pvInstance2->getCFrame().toWorldSpace(Box(-modifiedSize2, modifiedSize2));

			AABox box1, box2;
			modifiedBox1.getBounds(box1);
			modifiedBox2.getBounds(box2);

			bool areIntersecting = box1.intersects(box2);
			if (areIntersecting)
			{
				Array<Vector3> contactPoints, contactNormals;

				/* get two contact normals (for linkage) */
				CollisionDetection::penetrationDepthForFixedBoxFixedBox(modifiedBox1, modifiedBox2, contactPoints, contactNormals);

				for (int i = 0; i < contactNormals.size(); i++)
				{
					Vector3 normal = contactNormals[i];

					NormalId n0;
					n0 = fromNormal(normal);

					SurfaceType s0, s1;
					s0 = pvInstance1->getSurface(n0);
					s1 = pvInstance2->getSurface(PVInstance::getOppositeNormalId(n0));

					Linkage link = makeLinkage(s0, s1);

					if (link != NotLinked)
					{
						JointsService::singleton()->addConnector(fromLinkageAndPrimitives(link, pvInstance1->primitive, pvInstance2->primitive, n0));
					}
				}

			}
		}
	}
}

void RBX::JointsService::Experiment::buildGlobalJoints()
{
	/* create joints from contacts */
	getKernelWorldContacts();
}
