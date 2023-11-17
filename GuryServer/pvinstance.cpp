#include "pvinstance.h"
#include "lighting.h"
#include "pvenums.h"
#include "rbxmath.h"
#include "stdout.h"
#include "mesh.h"
#include "ray.h"

#define FRONT_CROSS Color3(0.35f, 0.35f, 0.35f)
#define BACK_CROSS  Color3(1,1,1)

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::PVInstance>("PVInstance")
         .constructor<>()
         .property("Anchored", &RBX::PVInstance::getAnchored, &RBX::PVInstance::setAnchored)
         .property("Locked", &RBX::PVInstance::getLocked, &RBX::PVInstance::setLocked)
         .property("CFrame", &RBX::PVInstance::getCFrame, &RBX::PVInstance::setCFrame)
         .property("Position", &RBX::PVInstance::getPosition, &RBX::PVInstance::setPosition)
         .property("size", &RBX::PVInstance::getSizeExternal, &RBX::PVInstance::setSize)
         .property("shape", &RBX::PVInstance::getShape, &RBX::PVInstance::setShape)
         .property("FrontSurface", &RBX::PVInstance::getFrontSurface, &RBX::PVInstance::setFrontSurface)
         .property("BackSurface", &RBX::PVInstance::getBackSurface, &RBX::PVInstance::setBackSurface)
         .property("TopSurface", &RBX::PVInstance::getTopSurface, &RBX::PVInstance::setTopSurface)
         .property("BottomSurface", &RBX::PVInstance::getBottomSurface, &RBX::PVInstance::setBottomSurface)
         .property("RightSurface", &RBX::PVInstance::getRightSurface, &RBX::PVInstance::setRightSurface)
         .property("LeftSurface", &RBX::PVInstance::getLeftSurface, &RBX::PVInstance::setLeftSurface)
         .property("rawFormFactor", &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor)
         .property("formFactor", &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor)
         .property("Transparency", &RBX::PVInstance::getTransparency, &RBX::PVInstance::setTransparency);
}

TextureRef texture_bumps;
TextureRef texture_studs;
TextureRef texture_inlets;
TextureRef texture_weld;

void RBX::initSurfaces()
{
    texture_weld = RBX::getSurface(RBX::Weld);
    texture_bumps = RBX::getSurface(RBX::Glue);
    texture_studs = RBX::getSurface(RBX::Studs);
    texture_inlets = RBX::getSurface(RBX::Inlet);
}

void drawFace(Vector2 uv, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    glNormal((v1 - v0).cross(v2 - v0).direction());

    glTexCoord2d(uv.x, uv.y);
    glVertex(v0);
    glTexCoord2d(0, uv.y);
    glVertex(v1);
    glTexCoord2d(0, 0);
    glVertex(v2);
    glTexCoord2d(uv.x, 0);
    glVertex(v3);
}

void RBX::PVInstance::setFace(NormalId f, SurfaceType s)
{
    switch (f)
    {
    case NormalId::TOP:
    {
        top = s;
        initFace()
        break;
    }
    case NormalId::BOTTOM:
    {
        bottom = s;
        break;
    }
    case NormalId::LEFT:
    {
        left = s;
        break;
    }
    case NormalId::RIGHT:
    {
        right = s;
        break;
    }
    case NormalId::FRONT:
    {
        front = s;
        break;
    }
    case NormalId::BACK:
    {
        back = s;
        break;
    }
    }
}

void RBX::PVInstance::initFace(unsigned int& f, SurfaceType s)
{
    int glid = -1;

    if (f == glid)
    {

        switch (s)
        {
        case Studs: glid = texture_studs->openGLID(); break;
        case Inlet: glid = texture_inlets->openGLID(); break;
        case Glue: glid = texture_bumps->openGLID(); break;
        case Weld: glid = texture_weld->openGLID(); break;
        default: break;
        }

        f = glid;

    }
}

void RBX::PVInstance::renderSurfaces(RenderDevice* rd)
{
    if (!specialShape)
    {
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

void RBX::renderSurface(RenderDevice* rd, RBX::PVInstance* pv, NormalId n, SurfaceType s, unsigned int& glid)
{
    pv->initFace(glid, s);
    if (glid != -1)
    {
        RBX::Render::rawDecal(rd, pv, n, glid, GL_SRC_ALPHA, GL_DST_COLOR);
    }
}

void RBX::PVInstance::render(RenderDevice* d)
{

    d->setObjectToWorldMatrix(getCFrame());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor(color.r, color.g, color.b, alpha);

    if (transparency < 1)
    {
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
            drawCylinderPluses(d);

            break;
        }
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_BLEND);

}

void RBX::PVInstance::renderFace(RenderDevice* rd, NormalId face, bool isDrawingDecal)
{

    glBegin(GL_QUADS);

    switch (face)
    {
    case NormalId::TOP:
    {
        drawFace(uv0,
            Vector3(size.x, size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(size.x, size.y, size.z));
        break;
    }
    case NormalId::BOTTOM:
    {
        drawFace(uv0,
            Vector3(size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    case NormalId::FRONT:
    {
        drawFace(uv1,
            Vector3(size.x, size.y, size.z),
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, -size.y, size.z),
            Vector3(size.x, -size.y, size.z));
    }
    case NormalId::BACK:
    {
        drawFace(uv1,
            Vector3(size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(size.x, size.y, -size.z));
        break;
    }
    case NormalId::LEFT:
    {
        drawFace(uv2,
            Vector3(-size.x, size.y, size.z),
            Vector3(-size.x, size.y, -size.z),
            Vector3(-size.x, -size.y, -size.z),
            Vector3(-size.x, -size.y, size.z));
        break;
    }
    case NormalId::RIGHT:
    {
        drawFace(uv2,
            Vector3(size.x, size.y, -size.z),
            Vector3(size.x, size.y, size.z),
            Vector3(size.x, -size.y, size.z),
            Vector3(size.x, -size.y, -size.z));
        break;
    }
    }

    glEnd();

}

void RBX::PVInstance::drawCylinderPluses(RenderDevice* d)
{
    float radius, axis;

    radius = getSize().x;
    axis = getSize().y;

    glRotatef(-90.0, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -axis * 0.5);
    glTranslatef(0.0, 0.0, axis);

    RBX::Primitives::drawLine(cylinderOriginX, d, FRONT_CROSS, lineWidth, -lineHeight);
    RBX::Primitives::drawLine(cylinderOriginX, d, FRONT_CROSS, -lineWidth, -lineHeight);
    RBX::Primitives::drawLine(cylinderOriginY, d, FRONT_CROSS, -lineHeight, lineWidth);
    RBX::Primitives::drawLine(cylinderOriginY, d, FRONT_CROSS, -lineHeight, -lineWidth);

    glRotatef(180.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, axis);

    RBX::Primitives::drawLine(cylinderOriginX, d, BACK_CROSS, lineWidth, -lineHeight);
    RBX::Primitives::drawLine(cylinderOriginX, d, BACK_CROSS, -lineWidth, -lineHeight);
    RBX::Primitives::drawLine(cylinderOriginY, d, BACK_CROSS, -lineHeight, lineWidth);
    RBX::Primitives::drawLine(cylinderOriginY, d, BACK_CROSS, -lineHeight, -lineWidth);

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

void RBX::PVInstance::calculateCylinderOffsets()
{
    float radius, scale;

    radius = getSize().x;
    scale = radius * 0.1;

    cylinderOriginX = Vector2(0, scale / 2);
    cylinderOriginY = Vector2(scale / 2, 0);

    lineWidth = radius / 1.5f;
    lineHeight = scale;
}

void RBX::PVInstance::calculateUvs()
{
    uv0 = Vector2(-size.x, size.z);
    uv1 = Vector2(-size.x, size.y);
    uv2 = Vector2(size.z, size.y);
}

void RBX::PVInstance::render3dSurface(RenderDevice* d, NormalId face)
{
    SurfaceType type;
    type = getSurface(face);

    if (type != SurfaceType::Smooth)
    {

        CoordinateFrame center;
        center = getSurfaceCenter(face, getSize(), getLocalExtents());

        d->setObjectToWorldMatrix(getCFrame());

        switch (type)
        {
        case SurfaceType::Hinge:
        {
            Draw::cylinder(Cylinder(center.translation - center.lookVector() * 0.5f, center.translation + center.lookVector() * 0.5f, 0.2f), d, Color3::yellow(), Color4::clear());
            break;
        }
        case SurfaceType::SteppingMotor:
        case SurfaceType::Motor:
        {

            Draw::cylinder(Cylinder(center.translation - center.lookVector() * 0.5f, center.translation + center.lookVector() * 0.5f, 0.2f), d, Color3::yellow(), Color4::clear());
            Draw::cylinder(Cylinder(center.translation, center.translation + center.lookVector() * 0.1f, 0.5f), d, Color3::gray(), Color4::clear());

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