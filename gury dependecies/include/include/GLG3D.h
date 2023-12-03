/**
 @file GLG3D.h

 This header includes all of the GLG3D libraries in 
 appropriate namespaces.

 @maintainer Morgan McGuire, matrix@graphics3d.com

 @created 2002-08-07
 @edited  2006-04-28

 Copyright 2001-2006, Morgan McGuire.
 All rights reserved.
*/

#ifndef G3D_GLG3D_H
#define G3D_GLG3D_H

#include "graphics3D.h"

// Set up the linker on Windows
#ifdef G3D_WIN32
    #ifndef GLG3D_BUILDING_LIBRARY_DLL
        #ifdef _DEBUG
            #pragma comment(lib, "GLG3D-debug.lib")        
        #else
            #pragma comment(lib, "GLG3D.lib")
        #endif
    #endif

    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")

    #pragma comment(lib, "sdl.lib")

    #if !defined(NO_SDL_MAIN) && !defined(_CONSOLE)
        #pragma comment(lib, "sdlmain.lib")
    #endif
#endif

#include "GLG3D/glheaders.h"
#include "GLG3D/glcalls.h"
#include "GLG3D/getOpenGLState.h"
#include "GLG3D/TextureFormat.h"
#include "GLG3D/Texture.h"
#include "GLG3D/glFormat.h"
#include "GLG3D/Milestone.h"
#include "GLG3D/RenderDevice.h"
#include "GLG3D/VARArea.h"
#include "GLG3D/VAR.h"
#include "GLG3D/GFont.h"
#include "GLG3D/LightingParameters.h"
#include "GLG3D/Sky.h"
#include "GLG3D/UserInput.h"
#include "GLG3D/ManualCameraController.h"
#include "GLG3D/VertexProgram.h"
#include "GLG3D/Draw.h"
#include "GLG3D/tesselate.h"
#include "GLG3D/GApp.h"
#include "GLG3D/PosedModel.h"
#include "GLG3D/IFSModel.h"
#include "GLG3D/MD2Model.h"
#include "GLG3D/TextureManager.h"
#include "GLG3D/shadowVolume.h"
#include "GLG3D/GWindow.h"
#include "GLG3D/SDLWindow.h"
#include "GLG3D/edgeFeatures.h"
#include "GLG3D/Shader.h"
#include "GLG3D/GLCaps.h"
#include "GLG3D/Shape.h"
#include "GLG3D/Renderbuffer.h"
#include "GLG3D/Framebuffer.h"
#include "GLG3D/GModule.h"

#ifdef G3D_WIN32
#include "GLG3D/Win32Window.h"
#include "GLG3D/DXCaps.h"
#endif

#ifdef G3D_LINUX
#include "GLG3D/X11Window.h"
#endif


#endif
