/**
  @file Texture.h

  @maintainer Morgan McGuire, matrix@graphics3d.com

  @created 2001-02-28
  @edited  2006-02-28
*/

#ifndef GLG3D_TEXTURE_H
#define GLG3D_TEXTURE_H

#include "G3D/ReferenceCount.h"
#include "G3D/Array.h"
#include "GLG3D/glheaders.h"
#include "GLG3D/TextureFormat.h"
#include "G3D/Vector2.h"

namespace G3D {

class GImage;
class Rect2D;
class Matrix3;

typedef ReferenceCountedPointer<class Texture> TextureRef;

/**
 Abstraction of OpenGL textures.  This class can be used with raw OpenGL, 
 without RenderDevice.  G3D::Texture supports all of the image formats
 that G3D::GImage can load, and DDS (DirectX textures), and Quake-style cube 
 maps.

 If you enable texture compression, textures will be compressed on the fly.
 This can be slow (up to a second).

 Unless DIM_2D_RECT, DIM_2D_NPOT, DIM_CUBE_MAP_NPOT are used, the texture is automatically
 scaled to the next power of 2 along each dimension to meet hardware requirements, if not
 already a power of 2.  However, DIM_2D_NPOT and DIM_CUBE_MAP_NPOT will safely fallback to
 POT requirements if the ARB_non_power_of_two extension is not supported. Develoeprs can 
 check if this will happen by calling GLCaps::supports_GL_ARB_texture_non_power_of_two().

 Textures are loaded so that (0, 0) is the upper-left corner of the image.
 If you set the invertY flag, RenderDevice will automatically turn them upside
 down when rendering to allow a (0, 0) <B>lower</B>-left corner.  If you
 aren't using RenderDevice, you must change the texture matrix to have
 a -1 in the Y column yourself.

 DIM_2D_RECT requires the GL_EXT_texture_rectangle extension.
 Texture compression requires the EXT_texture_compression_s3tc extions.
 You can either query OpenGL for whether these are supported or
 use the G3D::GLCaps facility for doing so.

 G3D::Texture can be used with straight OpenGL, without G3D::RenderDevice, as
 follows:

 <PRE>
  TextureRef texture = new Texture("logo.jpg");

  ...
    
  GLint u = texture->getOpenGLTextureTarget();
  glEnable(u);
  glBindTexture(u, texture->getOpenGLID());
 </PRE>

 To use Texture with RenderDevice:

  <PRE>
  TextureRef texture = new Texture("logo.jpg");
  ...
  renderDevice->setTexture(0, texture);
  // (to disable: renderDevice->setTexture(0, NULL);)
  </PRE>


  3D MIP Maps are not supported because gluBuild3DMipMaps is not in all GLU implementations.

  See G3D::RenderDevice::setBlendFunc for important information about turning on alpha blending. 
 */
class Texture : public ReferenceCountedObject {
public:

    /** DIM_2D_NPOT and DIM_CUBE_MAP_NPOT attempt to use ARB_non_power_of_two texture support with POT fallback. */
    enum Dimension       {DIM_2D = 2, DIM_2D_RECT = 4, DIM_CUBE_MAP = 5, DIM_2D_NPOT = 6, DIM_CUBE_MAP_NPOT = 7};

    /** TRANSPARENT_BORDER provides a border of Color4(0,0,0,0) and clamps to it. */
    enum WrapMode        {TILE = 1, CLAMP = 0, TRANSPARENT_BORDER = 2};

    /**
     Trilinear mipmap is the best quality (and frequently fastest) mode.  The no-mipmap modes conserve memory.
     Non-interpolating ("Nearest") modes are generally useful only when packing lookup tables into textures
     for shaders.

     @deprecated NO_INTERPOLATION will be replaced by NEAREST_NO_MIPMAP in a future release.
     */
    enum InterpolateMode {
        TRILINEAR_MIPMAP = 3, 
        BILINEAR_MIPMAP = 4,
        NEAREST_MIPMAP = 5,

        BILINEAR_NO_MIPMAP = 2,
        NEAREST_NO_MIPMAP = 6,
        NO_INTERPOLATION = 1};

    /** A depth texture can automatically perform the depth comparison used for shadow mapping
        on a texture lookup.  The result of a texture lookup is thus the shadowed amount
        (which will be percentage closer filtered on newer hardware) and <I>not</I> the 
        actual depth from the light's point of view.
       
        This combines GL_TEXTURE_COMPARE_MODE_ARB and GL_TEXTURE_COMPARE_FUNC_ARB from
        http://www.nvidia.com/dev_content/nvopenglspecs/GL_ARB_shadow.txt

        For best results on percentage closer hardware (GeForceFX and Radeon9xxx or better), 
        create shadow maps as depth textures with BILINEAR_NO_MIPMAP sampling.

        See also G3D::RenderDevice::configureShadowMap and the Collision_Demo.
     */
    enum DepthReadMode {DEPTH_NORMAL = 0, DEPTH_LEQUAL = 1, DEPTH_GEQUAL = 2};

    /**
     Splits a filename around the '*' character-- used by cube maps to generate all filenames.
     */
    static void splitFilenameAtWildCard(
        const std::string&  filename,
        std::string&        filenameBeforeWildCard,
        std::string&        filenameAfterWildCard);


    /**
     All parameters of a texture that are independent of the underlying image data.
     */
    class Settings {
    public:

        /** Default is TRILINEAR_MIPMAP */
        InterpolateMode             interpolateMode;

        /** Default is TILE */
        WrapMode                    wrapMode;

        /** Default is DEPTH_NORMAL */
        DepthReadMode               depthReadMode;

        /** Default is 2.0 */
        float                       maxAnisotropy;

        /** Default is true */
        bool                        autoMipMap;

        /**
         Highest MIP-map level that will be used during rendering.  The highest
         level that actually exists will be L = log(max(width, height), 2)), although
         it is fine to set maxMipMap higher than this.  Must be larger than minMipMap.
         Default is 1000.

         Setting the max mipmap level is useful for preventing adjacent areas of
         a texture from being blurred together when viewed at a distance.  It may
         decrease performance, however, by forcing a larger texture into cache
         than would otherwise be required.
         */
        int                         maxMipMap;

        /**
         Lowest MIP-map level that will be used during rendering.  Level 0 is the
         full-size image.  Default is -1000, matching the OpenGL spec.
         @cite http://oss.sgi.com/projects/ogl-sample/registry/SGIS/texture_lod.txt
         */
        int                         minMipMap;

        Settings();

        static const Settings& defaults();

        /** 
          Useful defaults for video/image processing.
          BILINEAR_NO_MIPMAP / CLAMP / DEPTH_NORMAL / 1.0 / false
        */
        static const Settings& video();

        /** 
          Useful defaults for shadow maps.
          BILINEAR_NO_MIPMAP / CLAMP / DEPTH_LEQUAL / 1.0 / false
        */
        static const Settings& shadow();

        /*
         Coming in a future version...
        void serialize(class BinaryOutput& b);
        void deserialize(class BinaryInput& b);
        void serialize(class TextOutput& t);
        void deserialize(class TextInput& t);
        */

        uint32 hashCode() const;
    };

    /**
     @deprecated Use Texture::Settings
     */
    typedef Settings Parameters;

    class PreProcess {
    public:

        /** Amount to brighten colors by (useful for Quake textures, which are dark). */
        float                       brighten;

        /** Amount to resize images by before loading to save memory; 
            typically a negative power of 2 (e.g., 1.0, 0.5, 0.25). */
        float                       scaleFactor;

        PreProcess() : brighten(1.0f), scaleFactor(1.0f) {}

        static PreProcess& defaults() {
            static PreProcess p;
            return p;
        }

    };

private:

    /** OpenGL texture ID */
	GLuint                          textureID;

    /** TODO: Remove */
    DepthReadMode                   _depthRead;
    /** TODO: Remove */
    float                           _maxAnisotropy;
    /** TODO: Remove */
    InterpolateMode                 interpolate;
    /** TODO: Remove */
    WrapMode                        wrap;

    /** Duplicates the values above.  Set in the base constructor. */
    Settings                        _settings;

    std::string                     name;
    Dimension                       dimension;
    bool                            _opaque;

    const class TextureFormat*      format;
    int                             width;
    int                             height;
    int                             depth;

    static size_t                   _sizeOfAllTexturesInMemory;

    Texture(
        const std::string&          _name,
        GLuint                      _textureID,
        Dimension                   _dimension,
        const class TextureFormat*  _format,
        InterpolateMode             _interpolate,
        WrapMode                    _wrap,
        bool                        _opaque,
        DepthReadMode               _depthRead,
        float                       _anisotropy);

public:

    /**
     Returns a new OpenGL texture ID.
     */
    static unsigned int newGLTextureID();

    /**
     Creates an empty texture (useful for later reading from the screen).
     */
    static TextureRef G3D_DEPRECATED createEmpty(
        int                             width,
        int                             height,
        const std::string&              name           = "Texture",
        const class TextureFormat*      desiredFormat  = TextureFormat::RGBA8,
        WrapMode                        wrap           = TILE,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0);

    /**
     Creates an empty texture (useful for later reading from the screen).
     */
    inline static TextureRef createEmpty(
        const std::string&              name,
        int                             width,
        int                             height,
        const class TextureFormat*      desiredFormat  = TextureFormat::RGBA8,
        Dimension                       dimension      = DIM_2D,
        const Parameters&               param          = Parameters::defaults()) {
        return Texture::createEmpty(width, height, name, desiredFormat, param.wrapMode, 
               param.interpolateMode, dimension, param.depthReadMode, param.maxAnisotropy);
    }

    /**
     Wrap and interpolate will override the existing parameters on the
     GL texture.

     @param name Arbitrary name for this texture to identify it
     @param textureID Set to newGLTextureID() to create an empty texture.
     @param maxAnisotropy Values over 1.0 will give better rendering of textures on shallow angles
     */
    static TextureRef fromGLTexture(
        const std::string&              name,
        GLuint                          textureID,
        const class TextureFormat*      textureFormat,
        WrapMode                        wrap           = TILE,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0f);


    static TextureRef G3D_DEPRECATED fromFile(
        const std::string&              filename,
        const class TextureFormat*      desiredFormat,
        WrapMode                        wrap,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        double                          brighten       = 1.0,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0f,
        float                           resizeFactor   = 1.0f);

    /**
     Creates a texture from a single image.  The image must have a format understood
     by G3D::GImage.  If dimension is DIM_CUBE_MAP, this loads the 6 files with names
     _ft, _bk, ... following the G3D::Sky documentation.
     @param brighten A value to multiply all color channels by; useful for loading
            dark Quake textures.
     */    
    inline static TextureRef fromFile(
        const std::string&              filename,
        const class TextureFormat*      desiredFormat  = TextureFormat::AUTO,
        Dimension                       dimension      = DIM_2D,
        const Parameters&               param          = Parameters::defaults(),
        const PreProcess&               process        = PreProcess()) {
        return Texture::fromFile(
            filename, 
            desiredFormat, 
            param.wrapMode, 
            param.interpolateMode, 
            dimension, process.brighten, 
            param.depthReadMode, 
            param.maxAnisotropy, 
            process.scaleFactor);
    }

    /**
     Creates a cube map from six independently named files.  The first
     becomes the name of the texture.
     */
    static TextureRef fromFile(
        const std::string               filename[6],
        const class TextureFormat*      desiredFormat  = TextureFormat::AUTO,
        WrapMode                        wrap           = TILE,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        double                          brighten       = 1.0,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0,
        float                           resizeFactor   = 1.0f);

    /**
     Creates a texture from the colors of filename and takes the alpha values
     from the red channel of alpha filename. See G3D::RenderDevice::setBlendFunc
	 for important information about turning on alpha blending. 
     */
    static TextureRef fromTwoFiles(
        const std::string&              filename,
        const std::string&              alphaFilename,
        const class TextureFormat*      desiredFormat  = TextureFormat::RGBA8,
        WrapMode                        wrap           = TILE,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0);

    /**
    bytes[miplevel][cubeface] is a pointer to the bytes
    for that miplevel and cube
    face. If the outer array has only one element and the
    interpolation mode is
    TRILINEAR_MIPMAP, mip-maps are automatically generated.
    There must be exactly
    6 cube faces per mip-level if the dimensions are
    DIM_CUBE and and 1 per
    mip-level otherwise. You may specify compressed and
    uncompressed formats for
    both the bytesformat and the desiredformat.

    depth must be 1 (reserved for future 3D textures).
    */
    static TextureRef fromMemory(
        const std::string&                  name,
        const Array< Array<const void*> >&  bytes,
        const TextureFormat*                bytesFormat,
        int                                 width,
        int                                 height,
        int                                 depth,
        const TextureFormat*                desiredFormat  = TextureFormat::AUTO,
        WrapMode                            wrap           = TILE,
        InterpolateMode                     interpolate    = TRILINEAR_MIPMAP,
        Dimension                           dimension      = DIM_2D,
        DepthReadMode                       depthRead      = DEPTH_NORMAL,
        float                               maxAnisotropy  = 2.0,
        float                               resizeFactor   = 1.0f);

    /**
     The bytes are described by byteFormat, which may differ from the
     format you want the graphics card to use (desiredFormat).
     If dimenion is DIM_CUBE_MAP bytes is an array of six images (for the faces)
     in the order: {FT, BK, UP, DN, RT, LF}.  Otherwise bytes is a pointer to
     an array of data.  Note that all faces must have the same dimensions and
     format for cube maps.
     */
    static TextureRef fromMemory(
        const std::string&              name,
        const uint8**                   bytes,
        const class TextureFormat*      bytesFormat,
        int                             width,
        int                             height,
        int                             depth,
        const class TextureFormat*      desiredFormat  = TextureFormat::AUTO,
        WrapMode                        wrap           = TILE,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0,
        float                           resizeFactor   = 1.0f);

    /** Use the constructor that accepts Texture::Parameters. */
	static TextureRef G3D_DEPRECATED fromMemory(
        const std::string&              name,
        const uint8*                    bytes,
        const class TextureFormat*      bytesFormat,
        int                             width,
        int                             height,
        const class TextureFormat*      desiredFormat,
        WrapMode                        wrap,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0,
        float                           resizeFactor   = 1.0f);

	 static TextureRef fromMemory(
        const std::string&              name,
        const uint8*                    bytes,
        const class TextureFormat*      bytesFormat,
        int                             width,
        int                             height,
        const class TextureFormat*      desiredFormat  = TextureFormat::AUTO,
        Dimension                       dimension = DIM_2D,
        const Parameters&               param = Parameters::defaults());


    /** Use the constructor that accepts Texture::Parameters. */
    static TextureRef G3D_DEPRECATED fromGImage(
        const std::string&              name,
        const GImage&                   image,
        const class TextureFormat*      desiredFormat,
        WrapMode                        wrap,
        InterpolateMode                 interpolate    = TRILINEAR_MIPMAP,
        Dimension                       dimension      = DIM_2D,
        DepthReadMode                   depthRead      = DEPTH_NORMAL,
        float                           maxAnisotropy  = 2.0);

    inline static TextureRef fromGImage(
        const std::string&              name,
        const GImage&                   image,
        const class TextureFormat*      desiredFormat  = TextureFormat::AUTO,
        Dimension                       dimension      = DIM_2D,
        const Parameters&               param = Parameters::defaults()) {
        return fromGImage(name, image, desiredFormat, param.wrapMode, param.interpolateMode, dimension, param.depthReadMode, param.maxAnisotropy);
    }

    /** Creates another texture that is the same as this one but contains only
        an alpha channel.  Alpha-only textures are useful as mattes.  
        
        If the current texture is opaque(), returns NULL (since it is not useful
        to construct an alpha-only version of a texture without an alpha channel).
        
        Like all texture construction methods, this is fairly
        slow and should not be called every frame during interactive rendering.*/
    TextureRef alphaOnlyVersion() const;

    /**
     Copies data from screen into an existing texture (replacing whatever was
     previously there).  The dimensions must be powers of two or a texture 
     rectangle will be created (not supported on some cards).

     The (x, y) coordinates are in real screen pixels.  (0, 0) is the top left
     of the screen.

     The texture dimensions will be updated but all other properties will be preserved:
     The previous wrap mode will be preserved.
     The interpolation mode will be preserved (unless it required a mipmap,
     in which case it will be set to BILINEAR_NO_MIPMAP).  The previous color depth
     and alpha depth will be preserved.  Texture compression is not supported for
     textures copied from the screen.

     To copy a depth texture, first create an empty depth texture then copy into it.

     If you invoke this method on a texture that is currently set on RenderDevice,
     the texture will immediately be updated (there is no need to rebind).

     @param useBackBuffer If true, the texture is created from the back buffer.
     If false, the texture is created from the front buffer.

     @param rect The rectangle to copy (relative to the viewport)
     See also RenderDevice::screenShotPic
     */
    void copyFromScreen(const Rect2D& rect, bool useBackBuffer = true);

    /**
     Argument for copyFromScreen
     */
    enum CubeFace {
        CUBE_POS_X = 0,
        CUBE_NEG_X = 1,
        CUBE_POS_Y = 2,
        CUBE_NEG_Y = 3,
        CUBE_POS_Z = 4,
        CUBE_NEG_Z = 5};

    /**
     Copies into the specified face of a cube map.  Because cube maps can't have
     the Y direction inverted (and still do anything useful), you should render
     the cube map faces <B>upside-down</B> before copying them into the map.  This
     is an unfortunate side-effect of OpenGL's cube map convention.  
     
     Use G3D::Texture::getCameraRotation to generate the (upside-down) camera
     orientations.
     */
    void copyFromScreen(const Rect2D& rect, CubeFace face, bool useBackBuffer = true);

    /**
     Returns the rotation matrix that should be used for rendering the
     given cube map face.
     */
    static void getCameraRotation(CubeFace face, Matrix3& outMatrix);

    /**
     When true, rendering code that uses this texture is respondible for
     flipping texture coordinates applied to this texture vertically (initially,
     this is false).
     
     RenderDevice watches this flag and performs the appropriate transformation.
     If you are not using RenderDevice (or are writing shaders), you must do it yourself.
     */
    bool invertY;

    /**
     How much (texture) memory this texture occupies.  OpenGL backs
     video memory textures with main memory, so the total memory 
     is actually twice this number.
     */
    size_t sizeInMemory() const;

    /**
     Video memory occupied by all OpenGL textures allocated using Texture
     or maintained by pointers to a Texture.
     */
    inline static size_t sizeOfAllTexturesInMemory() {
        return _sizeOfAllTexturesInMemory;
    }

    /**
     True if this texture was created with an alpha channel.  Note that
     a texture may have a format that is not opaque (e.g. RGBA8) yet still
     have a completely opaque alpha channel, causing texture->opaque to
     be true.  This is just a flag set for the user's convenience-- it does
     not affect rendering in any way.
	 See G3D::RenderDevice::setBlendFunc
	 for important information about turning on alpha blending. 
     */
    inline bool opaque() const {
        return _opaque;
    }

    /** @deprecated use Texture::parameters. */
    inline DepthReadMode G3D_DEPRECATED depthReadMode() const {
        return _depthRead;
    }

    /** @deprecated use Texture::parameters. */
    inline float G3D_DEPRECATED maxAnisotropy() const {
        return _maxAnisotropy;
    }

    inline unsigned int G3D_DEPRECATED getOpenGLID() const {
        return textureID;
    }

    /**
     Returns the level 0 mip-map data in the format that most closely matches
     outFormat.
     @param outFormat Must be one of: TextureFormat::AUTO, TextureFormat::RGB8, TextureFormat::RGBA8, TextureFormat::L8, TextureFormat::A8
     */
    void getImage(GImage& dst, const TextureFormat* outFormat = TextureFormat::AUTO) const;

    inline unsigned int openGLID() const {
        return textureID;
    }

	/** @deprecated Use texelWidth */
    inline const int G3D_DEPRECATED getTexelWidth() const {
        return width;
    }

	/** @deprecated Use texelHeight */
    inline const int G3D_DEPRECATED getTexelHeight() const {
        return height;
    }

    /**
     For 3D textures.
	 @deprecated Use texelDepth
     */
    inline const int G3D_DEPRECATED getTexelDepth() const {
        return depth;
    }

    inline const int texelWidth() const {
        return width;
    }

    inline const int texelHeight() const {
        return height;
    }

    inline Vector2 vector2Bounds() const {
        return Vector2((float)width, (float)height);
    }

    /** Returns a rectangle whose width and height match the dimensions of the texture. */
    Rect2D rect2DBounds() const;

    /**
     For 3D textures.
     */
    inline const int texelDepth() const {
        return depth;
    }

    inline const std::string& getName() const {
        return name;
    }

    /** @deprecated use Texture::parameters. */
    inline InterpolateMode G3D_DEPRECATED getInterpolateMode() const {
        return interpolate;
    }

	/** @deprecated use Texture::parameters. */
    inline WrapMode getWrapMode() const {
        return wrap;
    }

    /** @deprecated use Texture::parameters. */
    inline WrapMode G3D_DEPRECATED wrapMode() const {
        return wrap;
    }

    inline const TextureFormat* getFormat() const {
        return format;
    }
    
    inline Dimension G3D_DEPRECATED getDimension() const {
        return dimension;
    }

    /**
     Deallocates the OpenGL texture.
     */
    virtual ~Texture();

    /**
     The OpenGL texture target this binds (e.g. GL_TEXTURE_2D)
     */
    unsigned int getOpenGLTextureTarget() const;

    /**
     @deprecated Use settings()
     */
    const Settings& parameters() const;

    const Settings& settings() const;

    /** Set the autoMipMap value, which only affects textures when they are rendered 
        to or copied from the screen.

        You can read the automipmap value from <code>settings().autoMipMap</code>. 
        */
    void setAutoMipMap(bool b);

    /** For a texture with automipmap off that supports the FrameBufferObject extension, 
       generate mipmaps from the level 0 mipmap immediately.  For other textures, does nothing.*/
    void generateMipMaps();

private:

    class DDSTexture {
    private:
                                    
        uint8*                      bytes;
        const TextureFormat*        bytesFormat;
        int                         width;
        int                         height;
        int                         numMipMaps;
        int                         numFaces;

    public:

        DDSTexture(const std::string& filename);

        ~DDSTexture();

        int getWidth() {
            return width;
        }

        int getHeight() {
            return height;
        }

        const TextureFormat* getBytesFormat() {
            return bytesFormat;
        }

        int getNumMipMaps() {
            return numMipMaps;
        }

        int getNumFaces() {
            return numFaces;
        }

        uint8* getBytes() {
            return bytes;
        }
    };
};
/*
inline bool G3D_DEPRECATED operator==(const TextureRef& a, const void* b) {
    return (b == NULL) && (a == (TextureRef)NULL);
}

inline bool G3D_DEPRECATED operator==(const void* a, const TextureRef& b) {
    return b == a;
}

inline bool G3D_DEPRECATED operator!=(const TextureRef& a, const void* b) {
    return !(a == b);
}

inline bool G3D_DEPRECATED operator!=(const void* b, const TextureRef& a) {
    return !(a == b);
}
*/

} // namespace

G3D::uint32 hashCode(const G3D::Texture::Parameters& p);

#endif
