/**
  @file Sky.h

  @maintainer Morgan McGuire, morgan@cs.brown.edu

  @created 2002-10-04
  @edited  2006-02-11
*/

#ifndef G3D_SKY_H
#define G3D_SKY_H

#include "G3D/platform.h"
#include "GLG3D/Texture.h"
#include "GLG3D/RenderDevice.h"

namespace G3D {

typedef ReferenceCountedPointer<class Sky> SkyRef;

/**
 A background cube with an appropriately warped texture to make it 
 look like an infinite environment.  The sky also manages drawing
 the sun and moon, with lens flare effects for the sun.  
 
 Copy the images from data/sky to your project directory to use this
 class or provide your own.

 If you already have a preloaded CubeMap texture, use Sky::fromCubeMap.
   
 Example:

  <PRE>
    // Showing the default filename mask for clarity
    SkyRef sky = Sky::fromFile(renderDevice, "data/sky/", "plainsky/null_plainsky512_*.jpg");
    LightingParameters lighting(toSeconds(9, 00, 00, AM));

    ...

    // Rendering loop
        sky->render(lighting);
        
        // Draw the rest of the scene
        ...

        sky->renderLensFlare(lighting);
        ...
  </PRE>

 */
class Sky : public ReferenceCountedObject {
private:
    /** Indices into texture array */
    enum Direction {UP = 0, LT = 1, RT = 2, BK = 3, FT = 4, DN = 5};
	
    /**
     Only used if the render device does not support cube maps.
     */
    TextureRef                                  texture[6];

    /**
     Used unless the render device does not support cube maps.
     */
    TextureRef                                  cubeMap;

    TextureRef                                  sun;
    TextureRef                                  moon;
    TextureRef                                  disk;
    TextureRef                                  sunRays;

    Array<Vector4>                              star;
    Array<float>                                starIntensity;

    bool                                        drawCelestialBodies;

    /** @deprecated To be removed in 7.00 */
    class RenderDevice*                         renderDevice;

    /**
     Renders the sky box.
     */
    void renderBox(RenderDevice* rd) const;

    /** Draw the sun, called by render() */
    void drawSun(RenderDevice* rd, const class LightingParameters&);

    /** Draw the night sky, called by render() */
    void drawMoonAndStars(RenderDevice* rd, const class LightingParameters&);

    Sky(
        class RenderDevice*                     renderDevice,
        TextureRef                              textures[6],
        const std::string&                      directory,
        bool                                    useCubeMap,
        bool                                    drawCelestialBodies,
        double                                  quality);

    /** Draws a single sky-box vertex.  Called from renderBox. (s,t) are
        texture coordinates for the case where the cube map is not used.*/
    void vertex(RenderDevice* renderDevice, float x, float y, float z, float s, float t) const;

public:

    /**
     @param directory If directory is not "" it should
      end in a trailing slash.  This is the location of the real.str file
      and the sun, moon, etc. files.

     @param filename A filename with "*" in place of {up, lt, rt, bk, ft, dn}.
      The filename can be either fully qualified, relative to the current
      directory, or relative to <I>directory</I>.

     @param drawCelestialBodies If true, draw the sun, moon, and stars. Requires
      moon.jpg, moon-alpha.jpg, sun.jpg, lensflare.jpg, sun-rays.jpg be present
      in given directory. Defaults to true.

     @param quality Trade image quality for texture memory: 
       .5 -> 1/8  the texture memory of 1.0, 
        0 -> 1/16 the texture memory of 1.0.
        Color banding will occur at low quality settings.

     @param renderDevice May be NULL, if a non-NULL argument is provided to G3D::Sky::render.

     @param scaleDownFactor Resize the texture by 1/scaleDownFactor in each dimension.  Should be a power of two.
              Useful for creating a skybox that occupies less memory on low-end cards.
     */
    static SkyRef fromFile(
        class RenderDevice*                     renderDevice,
        const std::string&                      directory,
        const std::string&                      filename = "plainsky/null_plainsky512_*.jpg",
        bool                                    drawCelestialBodies = true,
        double                                  quality = 1.0,
        int                                     scaleDownFactor = 1);

    static SkyRef fromFile(
        class RenderDevice*                     renderDevice,
        const std::string&                      directory,
        const std::string                       filename[6],
        bool                                    drawCelestialBodies = true,
        double                                  quality = 1.0,
        int                                     scaleDownFactor = 1);

    /** @deprecated Converted to Sky::fromFile */
    static SkyRef G3D_DEPRECATED create(
        class RenderDevice*                     renderDevice,
        const std::string&                      directory,
        const std::string&                      filename = "plainsky/null_plainsky512_*.jpg",
        bool                                    drawCelestialBodies = true,
        double                                  quality = 1.0,
        int                                     scaleDownFactor = 1) {

        return Sky::fromFile(renderDevice, directory, filename, drawCelestialBodies, quality, scaleDownFactor);
    }

    /** @deprecated Converted to Sky::fromFile */
    static SkyRef G3D_DEPRECATED create(
        class RenderDevice*                     renderDevice,
        const std::string&                      directory,
        const std::string                       filename[6],
        bool                                    drawCelestialBodies = true,
        double                                  quality = 1.0,
        int                                     scaleDownFactor = 1) {

        return Sky::fromFile(renderDevice, directory, filename, drawCelestialBodies, quality, scaleDownFactor);
    }

    /**
     @param _cubeMap This must be a Texture of dimension - DIM_CUBE_MAP.
     */
    static SkyRef fromCubeMap(
        RenderDevice*                           rd,
        TextureRef                              _cubeMap,
        const std::string&                      directory,
        bool                                    _drawCelestialBodies = true,
        double                                  quality = 1.0);

    virtual ~Sky();

    /**
     Call at the very beginning of your rendering routine.
     Will restore all state it changes.
     */
	void render(
        RenderDevice* renderDevice,
        const class LightingParameters&         lighting);

    /** @deprecated Use render(RenderDevice, LightingParameters) */
    void render(
        const class LightingParameters&         lighting) {
        render(renderDevice, lighting);
    }

    /**
     Call at the very end of your rendering routine.
     Will restore all state it changes.
     */
    void renderLensFlare(
        RenderDevice*                           renderDevice,
        const class LightingParameters&         lighting);

    /** @deprecated Use renderLensFlare(RenderDevice, LightingParameters) */
    void renderLensFlare(
        const class LightingParameters&         lighting) {
        renderLensFlare(renderDevice, lighting);
    }

    /**
     Returns an environment cube map (or the front 2D texture if cube maps are not supported
     on this machine).
     */
    inline TextureRef getEnvironmentMap() const {
        if (cubeMap.isNull()) {
            return texture[FT];
        } else {
            return cubeMap;
        }
    }

};

inline bool operator==(const SkyRef& a, const void* b) {
    return (b == NULL) && (a == (SkyRef)NULL);
}

inline bool operator==(const void* a, const SkyRef& b) {
    return b == a;
}

inline bool operator!=(const SkyRef& a, const void* b) {
    return !(a == b);
}

inline bool operator!=(const void* b, const SkyRef& a) {
    return !(a == b);
}

}

#endif

