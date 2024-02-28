 #pragma once

#include <G3DAll.h>
#include <vector>

#include "kernel.h"
#include "primitive.h"

#include "decal.h"
#include "brickcolor.h"

#include "extents.h"

#include "IRenderable.h"
#include "ISelectable.h"

#include "surfaceFactory.h"
#include "stdout.h"

namespace RBX
{

	class PVInstance;
	class PartInstance;

	extern void renderSurface(RenderDevice* rd, RBX::PVInstance* pv, NormalId n, SurfaceType s, unsigned int& glid);
	extern CoordinateFrame getSurfaceCenter(NormalId face, Vector3 size, Extents e);
	extern TextureRef getSurface(RBX::SurfaceType s);
	extern float getAffectedFormFactor(RBX::PVInstance* pv);

	inline void initSurfaces()
	{
		RBX::RenderSurfaceFactory::initSurfaces();
	}
	 
	class PVInstance :
		public Render::Renderable, public RBX::ISelectable
	{
		friend class RBX::PartInstance;
	private:

		float elasticity, friction, erp, cfm;
		float lineWidth, lineHeight;
		float surfaceAlpha;

		Vector2 cylinderOriginX, cylinderOriginY,
			uv0, uv1, uv2;

		PV* startPV;

		unsigned int idFront, idBack, idTop, idBottom, idRight, idLeft;

	public:

		PV* pv;
		Primitive* primitive;
		Body* body;

		Vector3 size;
		Color3 color;

		SurfaceType front, back, top, bottom, right, left;

		FormFactor formFactor;

		bool showControllerFlag;
		bool canCollide;
		bool anchored;
		bool locked;

		float fauxTransparency; /* the version set by reflection */

		Shape shape;

	public:

		void resetPV()
		{
			setCFrame(startPV->position);
			setVelocity(startPV->velocity.linear);
			setRotVelocity(startPV->velocity.rotational);
		}

		void savePV()
		{
			startPV->position = getCFrame();
			startPV->velocity.linear = getVelocity();
			startPV->velocity.rotational = getRotVelocity();
		}

		void setShape(Shape s) 
		{
			shape = s;
			primitive->modifyShape(shape);
		}

		Shape getShape()
		{ 
			return shape; 
		}

		RBX::SurfaceType getFrontSurface() { return front; }
		void setFrontSurface(RBX::SurfaceType s) { setFace(FRONT, s); }

		RBX::SurfaceType getBackSurface() { return back; }
		void setBackSurface(RBX::SurfaceType s) { setFace(BACK, s); }

		RBX::SurfaceType getRightSurface() { return right; }
		void setRightSurface(RBX::SurfaceType s) { setFace(RIGHT, s); }

		RBX::SurfaceType getLeftSurface() { return left; }
		void setLeftSurface(RBX::SurfaceType s) { setFace(LEFT, s); }

		RBX::SurfaceType getTopSurface() { return top; }
		void setTopSurface(RBX::SurfaceType s) { setFace(TOP, s); }

		RBX::SurfaceType getBottomSurface() { return bottom; }
		void setBottomSurface(RBX::SurfaceType s) { setFace(BOTTOM, s); }

		Body* getBody()
		{
			if (primitive->body) return primitive->body;
			return 0;
		}

		void setVelocity(Vector3 newVelocity)
		{
			pv->velocity.linear = newVelocity;

			if (primitive->body)
			{
				primitive->body->modifyVelocity(pv->velocity);
			}
		}

		void setRotVelocity(Vector3 newVelocity)
		{
			pv->velocity.rotational = newVelocity;			

			if (primitive->body)
			{
				primitive->body->modifyVelocity(pv->velocity);
			}
		}

		void setAnchored(bool a)
		{
			anchored = a;
			if (anchored)
			{
				body->detachPrimitive(primitive);
			}
			else
			{
				body->attachPrimitive(primitive);
			}
		}

		void setCanCollide(bool c) 
		{
			canCollide = c;
			primitive->modifyCollisions(canCollide);
		}

		void setCenter(CoordinateFrame c)
		{
			setCFrame(c);
		}

		FormFactor getFormFactor() { return formFactor; }

		void setFormFactor(FormFactor f)
		{
			formFactor = f;
			setSize(getSizeExternal());
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

			if (primitive)
			{
				primitive->modifySize(size);
			}
			if (body)
			{
				body->modifySize(size);
			}
		}

		Vector3 getPosition() { return pv->position.translation; }

		void setPosition(Vector3 p) 
		{
			setCFrame(CoordinateFrame(pv->position.rotation, p));
		}

		CoordinateFrame getCFrame() { return pv->position; }

		void setCFrame(CoordinateFrame cf) 
		{
			pv->position = cf;
			primitive->modifyPosition(pv->position);
		}

		Color3 getColor() { return color; }
		void setColor(Color3 c) {color = c; }
		bool getAnchored() { return anchored; }
		bool getCanCollide() { return canCollide; }
		bool getLocked() { return locked; }

		SurfaceType getSurface(NormalId face);

		float getFauxTransparency()
		{
			return fauxTransparency;
		}

		float getTransparency()
		{ 
			return transparency;
		}
		void setTransparency(float f)
		{
			transparency = f; 
			alpha = 1 - transparency;
			fauxTransparency = f;
		}

		float getReflectance()
		{
			return reflectance;
		}

		void setReflectance(float r)
		{
			reflectance = r;
		}

		Vector3 getVelocity() { return pv->velocity.linear; }
		Vector3 getRotVelocity() { return pv->velocity.rotational; }

		Vector3 getEulerAngles()
		{
			Vector3 euler;
			pv->position.rotation.toEulerAnglesXYZ(euler.x, euler.y, euler.z);
			return euler;
		}

		void lookAt(Vector3 v)
		{
			CoordinateFrame cf;
			cf = getCFrame();
			cf.lookAt(v);
			setCFrame(cf);
		}

		void setLocked(bool l) { locked = l; }

		Box getBox() {
			Box box = Box(-getSize(), getSize());
			return getCFrame().toWorldSpace(box);
		}

		SelectableBox getBoundingBox()
		{
			return SelectableBox(pv->position, size);
		}

		CoordinateFrame getCenter()
		{
			return pv->position;
		}

		Instance* clone() const
		{
			return new PVInstance(*this);
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
		void initializeForKernel();

		PVInstance();

		RTTR_ENABLE(RBX::Render::Renderable)
	};

	namespace Primitives
	{
		extern void rawCylinderAlongX(Color4 color, float radius, float axis, int slices=12);
		extern void drawLine(Vector2 pos, RenderDevice* d, Color3 color, float width, float height);
		extern void drawBall(RenderDevice* d, RBX::PVInstance* base);
		extern void drawCylinder(RenderDevice* d, RBX::PVInstance* base);
	}
}