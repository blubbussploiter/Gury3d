#ifndef RENDER_SHAPES_H
#define RENDER_SHAPES_H

#include <G3DAll.h>
#include <vector>

#include "decal.h"
#include "brickcolor.h"

#include "runservice.h"

#include "extents.h"
#include "joints.h"

#include "IRenderable.h"
#include "ISelectable.h"

#include "surfaceFactory.h"

namespace RBX
{

	class PVInstance;

	extern void renderSurface(RenderDevice* rd, RBX::PVInstance* pv, NormalId n, SurfaceType s, unsigned int& glid);
	extern CoordinateFrame getSurfaceCenter(NormalId face, Vector3 size, Extents e);
	extern TextureRef getSurface(RBX::SurfaceType s);
	extern float getAffectedFormFactor(RBX::PVInstance* pv);

	inline void initSurfaces()
	{
		RBX::RenderSurfaceFactory::initSurfaces();
	}

	class PhysBody
	{
	public:
		int tag;
		btRigidBody* _body;
		btCollisionObject* _collider;
		btCollisionShape* _shape;
		RBX::Joint* connector;
		/* check if the body is colliding with anything -> 'preferredFace' = ignore any other faces that body is touching BESIDES the preferredFace */
		bool isColliding(NormalId preferredFace=UNDEFINED);
	};

	class PVInstance :
		public Render::Renderable, public RBX::ISelectable
	{
	private:

		float elasticity, friction, erp, cfm;
		float lineWidth, lineHeight;
		float surfaceAlpha;

		Vector2 cylinderOriginX, cylinderOriginY,
			uv0, uv1, uv2;

		unsigned int idFront, idBack, idTop, idBottom, idRight, idLeft;

	public:

		Vector3 size;
		Color3 color;

		CoordinateFrame cframe;

		Vector3 velocity;
		Vector3 rotVelocity;

		SurfaceType front, back, top, bottom, right, left;

		FormFactor formFactor;

		bool showControllerFlag;
		bool canCollide;
		bool anchored;
		bool locked;

		Shape shape;

	public:

		PhysBody* body;

		void setShape(Shape s) { shape = s; }
		Shape getShape() { return shape; }

		int getFrontSurface() { return front; }
		void setFrontSurface(int s) { setFace(FRONT, (RBX::SurfaceType)s); }

		int getBackSurface() { return back; }
		void setBackSurface(int s) { setFace(BACK, (RBX::SurfaceType)s); }

		int getRightSurface() { return right; }
		void setRightSurface(int s) { setFace(RIGHT, (RBX::SurfaceType)s); }

		int getLeftSurface() { return left; }
		void setLeftSurface(int s) { setFace(LEFT, (RBX::SurfaceType)s); }

		int getTopSurface() { return top; }
		void setTopSurface(int s) { setFace(TOP, (RBX::SurfaceType)s); }

		int getBottomSurface() { return bottom; }
		void setBottomSurface(int s) { setFace(BOTTOM, (RBX::SurfaceType)s); }

		void setVelocity(Vector3 newVelocity)
		{
			if (body->_body && newVelocity != Vector3::zero())
			{
				body->_body->setLinearVelocity(btVector3(newVelocity.x, newVelocity.y, newVelocity.z));
			}
			velocity = newVelocity;
		}

		void setRotVelocity(Vector3 newVelocity)
		{
			if (body->_body && newVelocity != Vector3::zero())
			{
				body->_body->setAngularVelocity(btVector3(newVelocity.x, newVelocity.y, newVelocity.z));
			}
			rotVelocity = newVelocity;
		}

		void setAnchored(bool a)
		{
			anchored = a;
			if (body->_body)
			{
				RBX::RunService::singleton()->getPhysics()->resetBody(this);
			}
		}
		void setCanCollide(bool c) 
		{
			canCollide = c; 
			if (body->_body)
			{
				RBX::RunService::singleton()->getPhysics()->resetBody(this);
			}
		}

		FormFactor getFormFactor() { return formFactor; }

		void setFormFactor(FormFactor f)
		{
			formFactor = f;
			setSize(getSize());
		}

		bool getShowControllerFlag() { return showControllerFlag; }
		void setShowControllerFlag(bool scf) { showControllerFlag = scf; }

		void setFace(NormalId f, SurfaceType s);
		void initFace(unsigned int& f,  SurfaceType s);

		virtual void render(RenderDevice* rd);
		virtual void renderFace(RenderDevice* rd, NormalId face);

		virtual void renderSurfaces(RenderDevice* rd);
		virtual void render3DSurfaces(RenderDevice* rd);

		void render3dSurface(RenderDevice* d, NormalId face);

		void drawCylinderPluses(RenderDevice* d);

		void calculateCylinderOffsets();
		void calculateUvs();

		Vector3 getSizeExternal()
		{
			return getSize() * 2;
		}

		Vector3 getSize()
		{
			return size;
		}

		void setSize(Vector3 s) {

			size = s;
			size /= 2;
			size.y *= getAffectedFormFactor(this);

			switch (shape)
			{
			case cylinder:
			{
				size.y *= 2;
				calculateCylinderOffsets();
				break;
			}
			case ball: break;
			}

			calculateUvs();

			RBX::RunService::singleton()->getPhysics()->removeBody(this);
			RBX::RunService::singleton()->getPhysics()->createBody(this);

		}

		Vector3 getPosition() { return cframe.translation; }

		void setPosition(Vector3 p) { setCFrame(CoordinateFrame(cframe.rotation, p)); }
		CoordinateFrame getCFrame() { return cframe; }

		void setCFrameNoPhysics(CoordinateFrame cf) { cframe = cf; }
		void setCFrame(CoordinateFrame cf) { cframe = cf; RBX::RunService::singleton()->getPhysics()->updateBodyCFrame(cframe, this); }

		Color3 getColor() { return color; }
		void setColor(Color3 c) {color = c; }
		bool getAnchored() { return anchored; }
		bool getCanCollide() { return canCollide; }
		bool getLocked() { return locked; }

		SurfaceType getSurface(NormalId face);

		float getTransparency() { return transparency; }
		void setTransparency(float f) { transparency = f; alpha = 1 - transparency; }

		Vector3 getVelocity() { return velocity; }
		Vector3 getRotVelocity() { return rotVelocity; }
		Vector3 getEulerRotation() { float x, y, z; cframe.rotation.toEulerAnglesXYZ(x, y, z); return Vector3(x, y, z); }
		void setEulerRotation(float x, float y, float z) { setCFrame(CoordinateFrame(Matrix3::fromEulerAnglesXYZ(x, y, z), getCFrame().translation)); }

		void lookAt(Vector3 v)
		{
			CoordinateFrame cf;
			cf = getCFrame();
			cf.lookAt(v);
			setCFrame(cf);
		}

		void setLocked(bool l) { locked = l; }

		Box getBox() {
			Box box = Box(Vector3(size.x, size.y, size.z), Vector3(-size.x, -size.y, -size.z));
			return getCFrame().toWorldSpace(box);
		}

		Box getBoundingBox()
		{
			return getBox();
		}

		Instance* clone() const {
			RBX::PVInstance* i = new PVInstance(*this);
			i->body = new PhysBody();
			RBX::RunService::singleton()->getPhysics()->createBody(i);
			return i;
		}

		Extents getLocalExtents() { return Extents(-getSize() / 2, getSize() / 2); }
		Extents getWorldExtents()
		{
			Extents localExtents = getLocalExtents();
			return localExtents.toWorldSpace(getCFrame());
		}

		float getElasticity() { return elasticity; }
		void setElasticity(float el) { elasticity = el; }
		float getFriction() { return friction; }
		void setFriction(float f) { friction = f; }
		float getERP() { return erp; }
		float getCFM() { return cfm; }
		void setCFM(float _cfm) { cfm = _cfm; }
		void setERP(float _erp) { erp = _erp; }

		void onRemove() { RBX::RunService::singleton()->getPhysics()->removeBody(this); }

		PVInstance()
		{
			cframe = CoordinateFrame(Vector3(0.f, 0.f, 0.f));
			size = Vector3(4.f, 1.2f, 2.f);
			color = Color3(0.639216f, 0.635294f, 0.643137f);

			setClassName("PVInstance");
			setName("PVInstance");

			idFront = -1;
			idBack = -1;
			idTop = -1;
			idBottom = -1;
			idRight = -1;
			idLeft = -1;

			isAffectedByPhysics = 1;

			elasticity = 0.5f;
			friction = 0.300000012f;

			body = new PhysBody();

			alpha = 1;

			setFormFactor(FormFactor::Symmetric);

			shape = part;
		}
		virtual ~PVInstance() { RBX::RunService::singleton()->getPhysics()->removeBody(this); }
		RTTR_ENABLE(RBX::Render::Renderable)
	};

	namespace Primitives
	{
		extern void rawCylinderAlongX(Color4 color, float radius, float axis);
		extern void drawLine(Vector2 pos, RenderDevice* d, Color3 color, float width, float height);
		extern void drawBall(RenderDevice* d, RBX::PVInstance* base);
		extern void drawCylinder(RenderDevice* d, RBX::PVInstance* base);
	}
}

#endif