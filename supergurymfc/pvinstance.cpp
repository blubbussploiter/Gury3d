#include "pvinstance.h"
#include "lighting.h"
#include "pvenums.h"
#include "rbxmath.h"
#include "stdout.h"
#include "mesh.h"
#include "ray.h"

#define CROSS Color3(0.5f, 0.5f, 0.5f)

/* dear user: I CANT WRITE 3D RENDER STUFF CODE!!! OKAY ITS A LITTLE SLOPPY BUT IT GETS THE JOB DONE! :)))))))))) */

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::PVInstance>("PVInstance")
         .constructor<>()
         .property("Anchored", &RBX::PVInstance::getAnchored, &RBX::PVInstance::setAnchored)(rttr::metadata("Type", RBX::Behavior))
         .property("CanCollide", &RBX::PVInstance::getCanCollide, &RBX::PVInstance::setCanCollide)(rttr::metadata("Type", RBX::Behavior))
         .property("Locked", &RBX::PVInstance::getLocked, 
             &RBX::PVInstance::setLocked)(rttr::metadata("Type", RBX::Behavior))
         .property("Position", &RBX::PVInstance::getPosition, &RBX::PVInstance::setPosition)(rttr::metadata("Type", RBX::Data))
         .property("Velocity", &RBX::PVInstance::getVelocity, &RBX::PVInstance::setVelocity)(rttr::metadata("Type", RBX::Data))
         .property("RotVelocity", &RBX::PVInstance::getRotVelocity, &RBX::PVInstance::setRotVelocity)(rttr::metadata("Type", RBX::Data))
         .property("Color", &RBX::PVInstance::getColor, &RBX::PVInstance::setColor)
         .property("CFrame", &RBX::PVInstance::getCFrame, &RBX::PVInstance::setCFrame)
         .property("CoordinateFrame", &RBX::PVInstance::getCFrame, &RBX::PVInstance::setCFrame)
         .property("Elasticity", &RBX::PVInstance::getElasticity, &RBX::PVInstance::setElasticity)(rttr::metadata("Type", RBX::Part))
         .property("Friction", &RBX::PVInstance::getFriction, &RBX::PVInstance::setFriction)(rttr::metadata("Type", RBX::Part))
         .property("Shape", &RBX::PVInstance::getShape, &RBX::PVInstance::setShape)(rttr::metadata("Type", RBX::Part))
         .property("shape", &RBX::PVInstance::getShape, &RBX::PVInstance::setShape)
         .property("size", &RBX::PVInstance::getSizeExternal, &RBX::PVInstance::setSize)(rttr::metadata("Type", RBX::Part))
         .property("Size", &RBX::PVInstance::getSizeExternal, &RBX::PVInstance::setSize)
         .property("FrontSurface", &RBX::PVInstance::getFrontSurface, &RBX::PVInstance::setFrontSurface)(rttr::metadata("Type", RBX::Surface))
         .property("BackSurface", &RBX::PVInstance::getBackSurface, &RBX::PVInstance::setBackSurface)(rttr::metadata("Type", RBX::Surface))
         .property("TopSurface", &RBX::PVInstance::getTopSurface, &RBX::PVInstance::setTopSurface)(rttr::metadata("Type", RBX::Surface))
         .property("BottomSurface", &RBX::PVInstance::getBottomSurface, &RBX::PVInstance::setBottomSurface)(rttr::metadata("Type", RBX::Surface))
         .property("RightSurface", &RBX::PVInstance::getRightSurface, &RBX::PVInstance::setRightSurface)(rttr::metadata("Type", RBX::Surface))
         .property("LeftSurface", &RBX::PVInstance::getLeftSurface, &RBX::PVInstance::setLeftSurface)(rttr::metadata("Type", RBX::Surface))
         .property("rawFormFactor", &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor)
         .property("formFactor", &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor)
         //.property("FormFactor", &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor)
         .property("Transparency", &RBX::PVInstance::getFauxTransparency, &RBX::PVInstance::setTransparency)(rttr::metadata("Type", RBX::Appearance));
}

/* hurl */

/* every side, no decals */

void drawFace(RBX::NormalId face, Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    glNormal((v0 - v2).cross(v1 - v3).direction());

    glTexCoord2d(uv.x, uv.y);
    glVertex(v0);
    glTexCoord2d(0, uv.y);
    glVertex(v1);
    glTexCoord2d(0, 0);
    glVertex(v2);
    glTexCoord2d(uv.x, 0);
    glVertex(v3);
}

/* every side, for surfaces */

void drawFaceStudsUV(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, Vector2 studs)
{
    glNormal((v0 - v2).cross(v1 - v3).direction());

    /* thumbs up */

    uv.x /= (studs.x / 2);
    uv.y /= (studs.y / 4);

    glTexCoord2d(0, uv.y);
    glVertex(v0);
    glTexCoord2d(uv.x, uv.y);
    glVertex(v1);
    glTexCoord2d(uv.x, 0);
    glVertex(v2);
    glTexCoord2d(0, 0);
    glVertex(v3);

}

/* (decals) bottom, back, left, right */

void drawFaceThree(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    glNormal((v0 - v2).cross(v1 - v3).direction());

    glTexCoord2d(uv.x, 0);
    glVertex(v0);
    glTexCoord2d(0, 0);
    glVertex(v1);
    glTexCoord2d(0, uv.y);
    glVertex(v2);
    glTexCoord2d(uv.x, uv.y);
    glVertex(v3);
}

/* (decals) front, top */

void drawFaceTwoOooOoOOoOoO(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    glNormal((v0 - v2).cross(v1 - v3).direction());

    glTexCoord2d(0, uv.y);
    glVertex(v0);
    glTexCoord2d(uv.x, uv.y);
    glVertex(v1);
    glTexCoord2d(uv.x, 0);
    glVertex(v2);
    glTexCoord2d(0, 0);
    glVertex(v3);
}

void RBX::PVInstance::setFace(NormalId f, SurfaceType s)
{
    switch (f)
    {
    case NormalId::TOP:
    {
        top = s;
        initFace(idTop, s);
        break;
    }
    case NormalId::BOTTOM:
    {
        bottom = s;
        initFace(idBottom, s);
        break;
    }
    case NormalId::LEFT:
    {
        left = s;
        initFace(idLeft, s);
        break;
    }
    case NormalId::RIGHT:
    {
        right = s;
        initFace(idRight, s);
        break;
    }
    case NormalId::FRONT:
    {
        front = s;
        initFace(idFront, s);
        break;
    }
    case NormalId::BACK:
    {
        back = s;
        initFace(idBack, s);
        break;
    }
    }
}

void RBX::PVInstance::initFace(unsigned int& f, SurfaceType s)
{
    if (f == -1)
    {
        f = RBX::RenderSurfaceFactory::getSurfaceTexture(s);
    }
}

void RBX::PVInstance::renderSurfaces(RenderDevice* rd)
{
    if (!specialShape)
    {
        glColor(1, 1, 1, 1 - (color.g * 0.2f));
        renderSurface(rd, this, TOP, top, idTop);
        renderSurface(rd, this, BOTTOM, bottom, idBottom);
        renderSurface(rd, this, RIGHT, right, idRight);
        renderSurface(rd, this, LEFT, left, idLeft);
        renderSurface(rd, this, FRONT, front, idFront);
        renderSurface(rd, this, BACK, back, idBack);
    }
}

void RBX::PVInstance::render3DSurfaces(RenderDevice* rd)
{

    render3dSurface(rd, TOP);
    render3dSurface(rd, BOTTOM);
    render3dSurface(rd, FRONT);
    render3dSurface(rd, BACK);
    render3dSurface(rd, LEFT);
    render3dSurface(rd, RIGHT);

}

void RBX::PVInstance::renderDecal(RenderDevice* rd, Decal* decal)
{
    if (!transparency)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    decal->render(rd, this);

    if (!transparency)
    {
        glBlendFunc(GL_ZERO, GL_ZERO);
        glDisable(GL_BLEND);
    }
}

void RBX::renderSurface(RenderDevice* rd, RBX::PVInstance* pv, NormalId n, SurfaceType s, unsigned int& glid)
{
    if (glid == -1) return;

        if (pv->transparency > 0) /* already blended, no need to reapply blend */
        {
            RBX::Render::rawDecal(rd, pv, n, glid);
        }
        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            RBX::Render::rawDecal(rd, pv, n, glid);
            glBlendFunc(GL_ZERO, GL_ZERO);
            glDisable(GL_BLEND);
        }
}

void RBX::PVInstance::render(RenderDevice* d)
{
    if (transparency < 1)
    {
        d->setObjectToWorldMatrix(getCFrame());
        d->setShininess(45.0f);

        glColor(color.r, color.g, color.b, alpha);

        switch (shape)
        {
        case part:
        {
            renderFace(d, TOP);
            renderFace(d, BOTTOM);
            renderFace(d, FRONT);
            renderFace(d, BACK);
            renderFace(d, LEFT);
            renderFace(d, RIGHT);

            renderSurfaces(d);
            
            break;
        }
        case ball:
        {
            RBX::Primitives::drawBall(d, this);
            break;
        }
        case cylinder:
        {
            RBX::Primitives::drawCylinder(d, this);
            break;
        }
        }
    }
    
}

void RBX::PVInstance::calculateCylinderOffsets()
{

    cylinderOriginX.y = getSize().magnitude() / 4;
    cylinderOriginY.x = getSize().magnitude() / 4;

}

void RBX::PVInstance::calculateUvs()
{
    uv0 = Vector2(size.x, -size.z / 2);
    uv1 = Vector2(-size.x, -size.y / 2);
    uv2 = Vector2(size.z, size.y / 2);
}

void RBX::PVInstance::renderFace(RenderDevice* rd, NormalId face)
{

    glBegin(GL_QUADS);

    switch (face)
    {
    case NormalId::TOP:
    {
        drawFace(face, uv0,
            Vector3(size.x, size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(size.x, size.y, size.z));
        break;
    }
    case NormalId::BOTTOM:
    {
        drawFace(face, uv0,
            Vector3(size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    case NormalId::FRONT:
    {
        drawFace(face, uv1,
            Vector3(size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(size.x, size.y, -size.z));
        break;
    }
    case NormalId::BACK:
    {
        drawFace(face, uv1,
            Vector3(size.x, size.y, size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(size.x, -size.y, size.z));
        break;
    }
    case NormalId::LEFT:
    {
        drawFace(face, uv2,
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, size.z));
        break;
    }
    case NormalId::RIGHT:
    {
        drawFace(face, uv2,
            Vector3(size.x, size.y, -size.z),
            Vector3(size.x, size.y, size.z),
            Vector3(size.x, -size.y, size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    }

    glEnd();

}

void RBX::PVInstance::renderFaceFitForDecal(RenderDevice* rd, NormalId face)
{
    Vector2 uv(1, 1);

    glBegin(GL_QUADS);

    switch (face)
    {
    case NormalId::TOP:
    {
        drawFaceTwoOooOoOOoOoO(uv,
            Vector3(size.x, size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(size.x, size.y, size.z));
        break;
    }
    case NormalId::BOTTOM:
    {
        drawFaceThree(uv,
            Vector3(size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    case NormalId::FRONT:
    {        
        drawFaceTwoOooOoOOoOoO(uv,
            Vector3(size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(size.x, size.y, -size.z));
        break;
    }
    case NormalId::BACK:
    {
        drawFaceThree(uv,
            Vector3(size.x, size.y, size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(size.x, -size.y, size.z));
        break;
    }
    case NormalId::LEFT:
    {
        drawFaceThree(uv,
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, size.z));
        break;
    }
    case NormalId::RIGHT:
    {
        drawFaceThree(uv,
            Vector3(size.x, size.y, -size.z),
            Vector3(size.x, size.y, size.z),
            Vector3(size.x, -size.y, size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    }

    glEnd();

}

void RBX::PVInstance::renderFaceFitForTexture(RenderDevice* rd, NormalId face, Vector2 StudsUV)
{

    glBegin(GL_QUADS);

    switch (face)
    {
    case NormalId::TOP:
    {
        drawFaceStudsUV(uv0,
            Vector3(size.x, size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(size.x, size.y, size.z), StudsUV);
        break;
    }
    case NormalId::BOTTOM:
    {
        drawFaceStudsUV(uv0,
            Vector3(size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(size.x, -size.y, -size.z), StudsUV);
        break;
    }
    case NormalId::FRONT:
    {
        drawFaceStudsUV(uv1,
            Vector3(size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(size.x, size.y, -size.z), StudsUV);
        break;
    }
    case NormalId::BACK:
    {
        drawFaceStudsUV(uv1,
            Vector3(size.x, size.y, size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(size.x, -size.y, size.z), StudsUV);
        break;
    }
    case NormalId::LEFT:
    {
        drawFaceStudsUV(uv2,
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, size.z), StudsUV);
        break;
    }
    case NormalId::RIGHT:
    {
        drawFaceStudsUV(uv2,
            Vector3(size.x, size.y, -size.z),
            Vector3(size.x, size.y, size.z),
            Vector3(size.x, -size.y, size.z),
            Vector3(size.x, -size.y, -size.z), StudsUV);
        break;
    }
    }

    glEnd();

}

void RBX::PVInstance::drawCylinderPluses(RenderDevice* d)
{
    float radius, axis, magnitude;

    radius = getSize().x;
    axis = getSize().y;

    magnitude = getSize().magnitude();

    float lineWidth = magnitude * 0.1f, lineHeight = magnitude * 0.25f;
    
    glPushMatrix();
    glRotatef(-90.0, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -axis * 0.5);
    glTranslatef(0.0, 0.0, axis);

    glTranslatef(0.0f, 0.0f, 0.01f);

    RBX::Primitives::drawLine(cylinderOriginX, d, CROSS, -lineWidth, lineHeight);

    glTranslatef(0.0f, 0.0f, -0.01f);

    glRotatef(180.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, axis);

    glTranslatef(0.0f, 0.0f, 0.01f);

    RBX::Primitives::drawLine(cylinderOriginX, d, CROSS, -lineWidth, lineHeight);
    
    glTranslatef(0.0f, 0.0f, -0.01f);
    glPopMatrix();
}

float RBX::getAffectedFormFactor(RBX::PVInstance* pv)
{
    switch (pv->getFormFactor())
    {
    case RBX::Brick:
    {
        return 1.2f;
    }
    case RBX::Plate:
    {
        return 0.4f;
    }
    case RBX::Symmetric:
    {
        return 1.0f;
    }
    }
}

void RBX::PVInstance::render3dSurface(RenderDevice* d, NormalId face)
{
    SurfaceType type;
    type = getSurface(face);

    if (type != SurfaceType::Smooth)
    {

        CoordinateFrame center, world;
        Vector3 worldSpace;
        Matrix3 worldRotation;

        center = getSurfaceCenter(face, getSize(), getLocalExtents());

        worldSpace = pv->position.pointToWorldSpace(center.translation);
        worldRotation = pv->position.rotation * center.rotation;

        world = CoordinateFrame(worldRotation, worldSpace);

        d->setObjectToWorldMatrix(world);

        switch (type)
        {
        case SurfaceType::Hinge:
        {
            RBX::Primitives::rawCylinderAlongX(Color3::yellow(), 0.2f, 1.0f, 6);
            break;
        }
        case SurfaceType::SteppingMotor:
        case SurfaceType::Motor:
        {

            RBX::Primitives::rawCylinderAlongX(Color3::gray(), 0.4f, 0.2f, 6); /* change for controller type in the future.... YES!!!! */
            RBX::Primitives::rawCylinderAlongX(Color3::yellow(), 0.2f, 1.0f, 6);

            break;
        }
        default:
        {
            break;
        }
        }

    }
}

RBX::SurfaceType RBX::PVInstance::getSurface(NormalId face)
{
    switch (face)
    {
    case NormalId::TOP:
    {
        return top;
    }
    case NormalId::BOTTOM:
    {
        return bottom;
    }
    case NormalId::FRONT:
    {
        return front;
    }
    case NormalId::BACK:
    {
        return back;
    }
    case NormalId::LEFT:
    {
        return left;
    }
    case NormalId::RIGHT:
    {
        return right;
    }
    }
    return Smooth;
}

void RBX::PVInstance::initializeForKernel()
{
    if (!body->created())
    {
        body->createBody(size);

        primitive->createPrimitive(shape, size);
        body->attachPrimitive(primitive);

        body->modifyUserdata(this);
        primitive->modifyUserdata(this);

        setAnchored(getAnchored());
        setCanCollide(getCanCollide());
    }
}

RBX::PVInstance::PVInstance()
{
    size = Vector3(4.f, 1.2f, 2.f);
    color = Color3(0.639216f, 0.635294f, 0.643137f);

    setClassName("PVInstance");
    setName("PVInstance");

    setFormFactor(FormFactor::Symmetric);

    idFront = -1;
    idBack = -1;
    idTop = -1;
    idBottom = -1;
    idRight = -1;
    idLeft = -1;

    elasticity = 0.5f;
    friction = 0.300000012f;

    canCollide = true;
    anchored = false;

    alpha = 1;
    shape = part;

    body = new Body();
    primitive = new Primitive(body);

    Kernel::get()->addPrimitive(primitive);

    pv = primitive->pv;
    startPV = new PV();

    fauxTransparency = 0;
}