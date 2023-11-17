/**
  @file TextureFormat.h

  @maintainer Morgan McGuire, matrix@graphics3d.com

  @created 2003-05-23
  @edited  2006-01-11
*/

#ifndef GLG3D_TEXTUREFORMAT_H
#define GLG3D_TEXTUREFORMAT_H

#include "G3D/platform.h"
#include "GLG3D/glheaders.h"

namespace G3D {

/**
 Used to describe texture formats to the G3D::Texture class.
 Don't construct these; use the static constants provided.

 @deprecated In 7.00, all static members will become functions with the 
 same name to avoid global initialization issues.
 */
class TextureFormat {
public:

	static bool valid;		// A hack to deal with app destruction. Set to false when static TextureFormats are deleted
    enum Code {
        CODE_L8,
        CODE_L16,
        CODE_L16F,
        CODE_L32F,

        CODE_A8,
        CODE_A16,
        CODE_A16F,
        CODE_A32F,

        CODE_LA4,
        CODE_LA8,
        CODE_LA16,
        CODE_LA16F,
        CODE_LA32F,

        CODE_RGB5,
        CODE_RGB5A1,
        CODE_RGB8,
        CODE_RGB16,
        CODE_RGB16F,
        CODE_RGB32F,

        CODE_ARGB8,
        CODE_BGR8,

        CODE_RGBA8,
        CODE_RGBA16,
        CODE_RGBA16F,
        CODE_RGBA32F,

        CODE_BAYER_RGGB8,
        CODE_BAYER_GRBG8,
        CODE_BAYER_GBRG8,
        CODE_BAYER_BGGR8,
        CODE_BAYER_RGGB32F,
        CODE_BAYER_GRBG32F,
        CODE_BAYER_GBRG32F,
        CODE_BAYER_BGGR32F,

        CODE_HSV8,
        CODE_HSV32F,

        CODE_YUV8,
        CODE_YUV32F,

        CODE_RGB_DXT1,
        CODE_RGBA_DXT1,
        CODE_RGBA_DXT3,
        CODE_RGBA_DXT5,

        CODE_DEPTH16,
        CODE_DEPTH24,
        CODE_DEPTH32,
        
        CODE_STENCIL1,
        CODE_STENCIL4,
        CODE_STENCIL8,
        CODE_STENCIL16
        };

    enum ColorSpace {
        COLOR_SPACE_NONE,
        COLOR_SPACE_RGB,
        COLOR_SPACE_HSV,
        COLOR_SPACE_YUV
    };

    enum BayerPattern {
        BAYER_PATTERN_NONE,
        BAYER_PATTERN_RGGB,
        BAYER_PATTERN_GRBG,
        BAYER_PATTERN_GBRG,
        BAYER_PATTERN_BGGR
    };

    /**
     Number of channels (1 for a depth texture).
     */
    int                 numComponents;
    bool                compressed;

    /** Useful for serializing */
    Code                code;

    ColorSpace          colorSpace;

    BayerPattern        bayerPattern;

    /**
     The GL format equivalent to this one.  Zero if there is no equivalent.
     */
    GLenum              OpenGLFormat;

    /**
     The GL base format equivalent to this one (e.g., GL_RGB, GL_ALPHA).  Zero if there is no equivalent.
     */
    GLenum              OpenGLBaseFormat;

    int                 luminanceBits;

    /**
     Number of bits per texel storage for alpha values; Zero for compressed textures.
     */
    int                 alphaBits;
    
    /**
     Number of bits per texel storage for red values; Zero for compressed textures.
     */
    int                 redBits;

    /**
     Number of bits per texel storage for green values; Zero for compressed textures.
     */
    int                 greenBits;
    
    /**
     Number of bits per texel storage for blue values; Zero for compressed textures.
     */
    int                 blueBits;

    int                 stencilBits;

    /**
     Number of depth bits (for depth textures; e.g. shadow maps)
     */
    int                 depthBits;

    /**
     Sum of the per-channel bits, plus any additional bits required
     for byte alignment.
     */
    int                 packedBitsPerTexel;
    
    /**
     This may be greater than the sum of the per-channel bits
     because graphics cards need to pad to the nearest 1, 2, or
     4 bytes.
     */
    int                 hardwareBitsPerTexel;

    /**
     True if there is no alpha channel for this texture.
     */
    bool                opaque;

    /** True if the bit depths specified are for float formats. */
    bool                floatingPoint;

private:

    TextureFormat(
        int             _numComponents,
        bool            _compressed,
        GLenum          _glFormat,
        GLenum          _glBaseFormat,
        int             _luminanceBits,
        int             _alphaBits,
        int             _redBits,
        int             _greenBits,
        int             _blueBits,
        int             _depthBits,
        int             _stencilBits,
        int             _hardwareBitsPerTexel,
        int             _packedBitsPerTexel,
        bool            _opaque,
        bool            _floatingPoint,
        Code            _code,
        ColorSpace      _colorSpace,
        BayerPattern    _bayerPattern = BAYER_PATTERN_NONE) : 
        numComponents(_numComponents),
        compressed(_compressed),
        code(_code),
        colorSpace(_colorSpace),
        bayerPattern(_bayerPattern),
        OpenGLFormat(_glFormat),
        OpenGLBaseFormat(_glBaseFormat),
        luminanceBits(_luminanceBits),
        alphaBits(_alphaBits),
        redBits(_redBits),
        greenBits(_greenBits),
        blueBits(_blueBits),
        stencilBits(_stencilBits),
        depthBits(_depthBits),
        packedBitsPerTexel(_packedBitsPerTexel),
        hardwareBitsPerTexel(_hardwareBitsPerTexel),
        opaque(_opaque),
        floatingPoint(_floatingPoint) {
    }

public:

    /**
     Default argument for TextureFormat::depth.
     */
    enum {SAME_AS_SCREEN = 0};

    static const TextureFormat* L8;

    static const TextureFormat* L16;

    static const TextureFormat* L16F;
    
    static const TextureFormat* L32F;

    static const TextureFormat* A8;

    static const TextureFormat* A16;

    static const TextureFormat* A16F;
    
    static const TextureFormat* A32F;

    static const TextureFormat* LA4;

    static const TextureFormat* LA8;

    static const TextureFormat* LA16;

    static const TextureFormat* LA16F;
    
    static const TextureFormat* LA32F;

    static const TextureFormat* RGB5;

    static const TextureFormat* RGB5A1;

    static const TextureFormat* RGB8;

    static const TextureFormat* RGB16;

    static const TextureFormat* RGB16F;

    static const TextureFormat* RGB32F;

    static const TextureFormat* RGBA8;

    static const TextureFormat* RGBA16;

    static const TextureFormat* RGBA16F;
    
    static const TextureFormat* RGBA32F;
    
    static const TextureFormat* RGB_DXT1;

    static const TextureFormat* RGBA_DXT1;

    static const TextureFormat* RGBA_DXT3;

    static const TextureFormat* RGBA_DXT5;

    static const TextureFormat* DEPTH16;

    static const TextureFormat* DEPTH24;

    static const TextureFormat* DEPTH32;

    static const TextureFormat* STENCIL1;

    static const TextureFormat* STENCIL4;

    static const TextureFormat* STENCIL8;

    static const TextureFormat* STENCIL16;
    
	/**
     NULL pointer; indicates that the texture class should choose
     either RGBA8 or RGB8 depending on the presence of an alpha channel
     in the input.
     */
    static const TextureFormat* AUTO;

    /**
     Returns DEPTH16, DEPTH24, or DEPTH32 according to the bits
     specified.
     */
    static const TextureFormat* depth(int depthBits = SAME_AS_SCREEN);

    /**
     Returns STENCIL1, STENCIL4, STENCIL8 or STENCIL16 according to the bits
     specified.
     */
    static const TextureFormat* stencil(int bits = SAME_AS_SCREEN);

    /**
     Returns the matching TextureFormat* identified by the Code.
     */
    static const TextureFormat* fromCode(TextureFormat::Code code);
};

}

inline unsigned int hashCode(const G3D::TextureFormat* f) {
#   if defined(G3D_WIN32)
        // Disable 64-bit pointer truncation warning
#       pragma warning (push)
#       pragma warning (disable : 4311)
#   endif	
    
    return reinterpret_cast<unsigned int>(f);

#   if defined(G3D_WIN32)
#       pragma warning (pop)
#   endif	
}

#endif
