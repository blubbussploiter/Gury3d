#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include <G3DAll.h>

#include "bullet/LinearMath/btIDebugDraw.h"
#include "pvinstance.h"
#include "bthelp.h"
#include "rbx.h"

class GLDebugDrawer : public btIDebugDraw
{
    int m_debugMode;

public:

    GLDebugDrawer() : m_debugMode(0) {}

    virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

    virtual void   reportErrorWarning(const char* warningString);

    virtual void   draw3dText(const btVector3& location, const char* textString);

    virtual void   setDebugMode(int debugMode);

    virtual int      getDebugMode() const { return m_debugMode; }

};
#endif