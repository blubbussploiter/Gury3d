
#include "debugDrawer.h"
#include "GuiRoot.h"

void    GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    //Draw::lineSegment(LineSegment::fromTwoPoints(b3Vector3(from), b3Vector3(to)), Rendering::global_app->renderDevice, Color3(color.x(), color.y(), color.z()));
}

void    GLDebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

void    GLDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
\
}

void    GLDebugDrawer::reportErrorWarning(const char* warningString)
{
    printf(warningString);
}

void    GLDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    //Draw::sphere(Sphere(b3Vector3(pointOnB), 0.5f), Rendering::global_app->renderDevice);
}