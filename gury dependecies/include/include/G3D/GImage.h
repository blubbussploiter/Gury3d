/**
  @file GImage.h

  See G3D::GImage for details.

  @cite JPEG compress/decompressor is the <A HREF="http://www.ijg.org/files/">IJG library</A>, used in accordance with their license.
  @cite JPG code by John Chisholm, using the IJG Library
  @cite TGA code by Morgan McGuire
  @cite BMP code by John Chisholm, based on code by Edward "CGameProgrammer" Resnick <A HREF="mailto:cgp@gdnmail.net">mailto:cgp@gdnmail.net</A> at <A HREF="ftp://ftp.flipcode.com/cotd/LoadPicture.txt">ftp://ftp.flipcode.com/cotd/LoadPicture.txt</A>
  @cite PCX format described in the ZSOFT PCX manual http://www.nist.fss.ru/hr/doc/spec/pcx.htm#2
  @cite PNG compress/decompressor is the <A HREF="http://www.libpng.org/pub/png/libpng.html">libpng library</A>, used in accordance with their license.
  @cite PPM code by Morgan McGuire based on http://netpbm.sourceforge.net/doc/ppm.html

  @maintainer Morgan McGuire, morgan@graphics3d.com
  @created 2002-05-27
  @edited  2006-01-10

  Copyright 2000-2006, Morgan McGuire.
  All rights reserved.

 */

#ifndef G3D_GIMAGE_H
#define G3D_GIMAGE_H

#include "G3D/platform.h"
#include <string>
#include "G3D/Array.h"
#include "G3D/g3dmath.h"
#include "G3D/stringutils.h"
#include "G3D/Color3uint8.h"
#include "G3D/Color4uint8.h"

namespace G3D {
class BinaryInput;
class BinaryOutput;
/**
  Interface to image compression & file formats. 
 
  Supported formats (decode and encode): Color JPEG, PNG, (Uncompressed)TGA 24, (Uncompressed)TGA 32, BMP 1, BMP 4, BMP 8, BMP 24, PPM (P6), and PPM ASCII (P1, P2, P3).
  8-bit paletted PCX, 24-bit PCX, and ICO are supported for decoding only.

  Sample usage:

  <PRE>
    #include "graphics3D.h"

    // Loading from disk:
    G3D::GImage im1 = G3D::Image("test.jpg");
    
    // Loading from memory:
    G3D::GImage im2 = G3D::GImage(data, length);

    // im.pixel is a pointer to RGB color data.  If you want
    // an alpha channel, call RGBtoRGBA or RGBtoARGB for
    // conversion.

    // Saving to memory:
    G3D::GImage im3 = G3D::GImage(width, height);
    // (Set the pixels of im3...) 
    uint8* data2;
    int    len2;
    im3.encode(G3D::GImage::JPEG, data2, len2);

    // Saving to disk
    im3.save("out.jpg");
  </PRE>

  The free Image Magick Magick Wand API 
  (http://www.imagemagick.org/www/api/magick_wand.html) provides a more powerful
  API for image manipulation and wider set of image load/save formats.  It is 
  recommended over GImage (we don't include it directly in G3D because their license
  is more restrictive than the BSD one).

  */
class GImage {
public:
    uint8*                _byte;

public:

    class Error {
    public:
        Error(
            const std::string& reason,
            const std::string& filename = "") :
        reason(reason), filename(filename) {}
        
        std::string reason;
        std::string filename;
    };

    enum Format {JPEG, BMP, TGA, PCX, ICO, PNG, PPM_ASCII, PPM, AUTODETECT, UNKNOWN};

    int                     width;
    int                     height;

    /**
     The number of channels; either 3 (RGB) or 4 (RGBA)
     */
    int                     channels;

    inline const uint8* byte() const {
        return _byte;
    }

    /** Returns a pointer to the upper left pixel
        as Color3uint8.
     */
    inline const Color3uint8* pixel3() const {
         debugAssertM(channels == 3, format("Tried to call GImage::pixel3 on an image with %d channels", channels));            
         return (Color3uint8*)_byte;
    }

    /** Returns a pointer to the upper left pixel
        as Color4uint8.
     */
    inline const Color4uint8* pixel4() const {
        debugAssertM(channels == 4, format("Tried to call GImage::pixel4 on an image with %d channels", channels));            
        return (Color4uint8*)_byte;
    }

    /** Returns the pixel at (x, y), where (0,0) is the upper left. */
    inline const uint8& pixel1(int x, int y) const {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return byte()[x + y * width];
    }

    /** Returns the pixel at (x, y), where (0,0) is the upper left. */
    inline uint8& pixel1(int x, int y) {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return byte()[x + y * width];
    }

    /** Returns the pixel at (x, y), where (0,0) is the upper left. */
    inline const Color3uint8& pixel3(int x, int y) const {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return pixel3()[x + y * width];
    }

    inline Color3uint8& pixel3(int x, int y) {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return pixel3()[x + y * width];
    }

    /** Returns the pixel at (x, y), where (0,0) is the upper left. */
    inline const Color4uint8& pixel4(int x, int y) const {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return pixel4()[x + y * width];
    }

    inline Color4uint8& pixel4(int x, int y) {
        debugAssert(x >= 0 && x < width);
        debugAssert(y >= 0 && y < height);
        return pixel4()[x + y * width];
    }

    inline uint8* byte() {
        return _byte;
    }

    inline Color3uint8* pixel3() {
        debugAssert(channels == 3);
        return (Color3uint8*)_byte;
    }

    inline Color4uint8* pixel4() {
        debugAssert(channels == 4);
        return (Color4uint8*)_byte;
    }

private:

    void encodeBMP(
        BinaryOutput&       out) const;

    /**
     The TGA file will be either 24- or 32-bit depending
     on the number of channels.
     */
    void encodeTGA(
        BinaryOutput&       out) const;

    /**
     Converts this image into a JPEG
     */
    void encodeJPEG(
        BinaryOutput&       out) const;

    /**
     Converts this image into a JPEG
     */
    void encodePNG(
        BinaryOutput&       out) const;

    void encodePPM(
        BinaryOutput&       out) const;

    void encodePPMASCII(
        BinaryOutput&       out) const;

    /**
     Decodes the buffer into this image.
     @format Guaranteed correct format.
     */
    void decode(
        BinaryInput&        input,
        Format              format);

    void decodeTGA(
        BinaryInput&        input);

    void decodeBMP(
        BinaryInput&        input);

    void decodeJPEG(
        BinaryInput&        input);

    void decodePCX(
        BinaryInput&        input);

    void decodeICO(
        BinaryInput&        input);

    void decodePNG(
        BinaryInput&        input);

    void decodePPM(
        BinaryInput&        input);

    void decodePPMASCII(
        BinaryInput&        input);

    /**
     Given [maybe] a filename, memory buffer, and [maybe] a format, 
     returns the most likely format of this file.
     */
    static Format resolveFormat(
        const std::string&  filename,
        const uint8*        data,
        int                 dataLen,
        Format              maybeFormat);

    void _copy(
        const GImage&       other);

public:
    static Format resolveFormat(const std::string& filename);

    GImage() {
        width = height = channels = 0;
        _byte = NULL;
    }

    /**
     Load an encoded image from disk and decode it.
     Throws GImage::Error if something goes wrong.
     */
    GImage(
        const std::string&  filename,
        Format              format = AUTODETECT);

    /**
     Decodes an image stored in a buffer.
    */
    GImage(
        const unsigned char*data,
        int                 length,
        Format              format = AUTODETECT);

    /**
     Create an empty image of the given size.
     */
    GImage(
        int                 width,
        int                 height,
        int                 channels = 3);

    GImage(
        const GImage&       other);

    GImage& operator=(const GImage& other);

    /**
     Returns a new GImage that has 4 channels.  RGB is
     taken from this GImage and the alpha from the red
     channel of the second image.
     */ 
    GImage insertRedAsAlpha(const GImage& alpha) const;

    /**
     Returns a new GImage with 3 channels, removing
     the alpha channel if there is one.
     */
    GImage stripAlpha() const;

    /**
     Loads an image from disk (clearing the old one first).
     */
    void load(
        const std::string&  filename,
        Format              format = AUTODETECT);

    /**
     Frees memory and resets to a 0x0 image.
     */
    void clear();
        
    /**
     Deallocates the pixels.
     */
    virtual ~GImage();

    /**
     Resizes the internal buffer to (width x height) with the
     number of channels specified.  All data is set to 0 (black).
     */
    void resize(int width, int height, int channels);


    /**
     Copies src sub-image data into dest at a certain offset.  
     The dest variable must already contain an image that is large
     enough to contain the src sub-image at the specified offset.
     Returns true on success and false if the src sub-image cannot
     completely fit within dest at the specified offset.  Both
     src and dest must have the same number of channels.
     */
    static bool pasteSubImage(GImage & dest, const GImage & src,
        int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight);

    /**
     creates dest from src sub-image data.  
     Returns true on success and false if the src sub-image
     is not within src.
     */
    static bool copySubImage(GImage & dest, const GImage & src,
        int srcX, int srcY, int srcWidth, int srcHeight);

    void convertToRGBA();

    void convertToRGB();

    /** Averages color channels if they exist */
    void convertToL8();

    /**
      Returns true if format is supported.  Format
      should be an extension string (e.g. "BMP").
     */
    static bool supportedFormat(
        const std::string& format);

    /**
     Converts a string to an enum, returns UNKNOWN if not recognized.
     */
    static Format stringToFormat(
        const std::string& format);

    /**
     Encode and save to disk.
     */
    void save(
        const std::string& filename,
        Format             format = AUTODETECT) const;
   
    /**
     The caller must delete the returned buffer.
     */
    void encode(
        Format              format,
        uint8*&             outData,
        int&                outLength) const;

    /**
     Does not commit the BinaryOutput when done.
     */
    void encode(
        Format              format,
        BinaryOutput&       out) const;

	/** Returns the size of this object in bytes */
	int sizeInMemory() const;


    /** Ok for in == out */
    static void R8G8B8_to_Y8U8V8(int width, int height, const uint8* in, uint8* out);

    /** Ok for in == out */
    static void Y8U8V8_to_R8G8B8(int width, int height, const uint8* in, uint8* out);

    /**
    @param in        RGB buffer of numPixels * 3 bytes
    @param out       Buffer of numPixels * 4 bytes
    @param numPixels Number of RGB pixels to convert
    */
    static void RGBtoRGBA(
        const uint8*            in,
        uint8*                  out,
        int                     numPixels);

    /** Safe for in == out */
    static void RGBtoBGR(
        const uint8*            in,
        uint8*                  out,
        int                     numPixels);

    /**
    Win32 32-bit HDC format.
    */
    static void RGBtoBGRA(
        const uint8*            in,
        uint8*                  out,
        int                     numPixels);

    static void RGBtoARGB(
        const uint8*            in,
        uint8*                  out,
        int                     numPixels);

    /**
    Uses the red channel of the second image as an alpha channel.
    */
    static void RGBxRGBtoRGBA(
        const uint8*            colorRGB,
        const uint8*            alphaRGB,
        uint8*                  out,
        int                     numPixels);
        
    /**
    Flips the image along the vertical axis.
    Safe for in == out.
    */
    static void flipRGBVertical(
        const uint8*            in,
        uint8*                  out,
        int                     width,
        int                     height);

    static void flipRGBAVertical(
        const uint8*            in,
        uint8*                  out,
        int                     width,
        int                     height);

    /**
    Given a tangent space bump map, computes a new image where the
    RGB channels are a tangent space normal map and the alpha channel
    is the original bump map.  Assumes the input image is tileable.

    In the resulting image, x = red = tangent, y = green = binormal, and z = blue = normal. 

    Particularly useful as part of the idiom:
    <PRE>
 	    GImage normal;
	    computeNormalMap(GImage(filename), normal);
	    return Texture::fromGImage(filename, normal);
    </PRE>

    @param lowPassBump If true, a 9x9 filter of 1's is used to low-pass filter the elevations,
        which produces better results for parallax mapping.

    @param scaleHeightByNz After computing normals, scale the height by |N.z|, a trick that
        reduces texture swim in steep areas for parallax mapping.

    @cite ATI demo
    */
    static void computeNormalMap(
        const class GImage& bump, 
        class GImage& normal,
        bool lowPassBump = false,
        bool scaleHeightByNz = false);

    /**
    Bayer demosaicing using the filter proposed in 

    HIGH-QUALITY LINEAR INTERPOLATION FOR DEMOSAICING OF BAYER-PATTERNED COLOR IMAGES
    Henrique S. Malvar, Li-wei He, and Ross Cutler

    The filter wraps at the image boundaries.

    Assumes in != out.
    */
    static void BAYER_G8B8_R8G8_to_R8G8B8_MHC(int w, int h, const uint8* in, uint8* _out);
    static void BAYER_G8R8_B8G8_to_R8G8B8_MHC(int w, int h, const uint8* in, uint8* _out);
    static void BAYER_R8G8_G8B8_to_R8G8B8_MHC(int w, int h, const uint8* in, uint8* _out);
    static void BAYER_B8G8_G8R8_to_R8G8B8_MHC(int w, int h, const uint8* in, uint8* _out);

    /** Fast conversion; the output has 1/2 the size of the input in each direction. Assumes in != out.
    See G3D::BAYER_G8B8_R8G8_to_R8G8B8_MHC for a much better result. */
    static void BAYER_G8B8_R8G8_to_Quarter_R8G8B8(int inWidth, int inHeight, const uint8* in, uint8* out);

    /** Attempt to undo fast conversion of G3D::BAYER_G8B8_R8G8_to_Quarter_R8G8B8; 
        the green channel will lose data. Assumes in != out 
        The input should have size 3 * inWidth * inHeight.  The output should have size
        2 * inWidth * 2 * inHeight.
    */
    static void Quarter_R8G8B8_to_BAYER_G8B8_R8G8(int inWidth, int inHeight, const uint8* in, uint8* out);

    /** Overwrites every pixel with one of the two colors in a checkerboard pattern.
        The fields used from the two colors depend on the current number of channels in @a im. 
        */
    static void makeCheckerboard(GImage& im, int checkerSize = 1, const Color4uint8& color1 = Color4uint8(255,255,255,255), const Color4uint8& color2 = Color4uint8(0,0,0,255));
};

   
/**
 @param in        RGB buffer of numPixels * 3 bytes
 @param out       Buffer of numPixels * 4 bytes
 @param numPixels Number of RGB pixels to convert
 */
inline void G3D_DEPRECATED RGBtoRGBA(
    const uint8*            in,
    uint8*                  out,
    int                     numPixels) {
    GImage::RGBtoRGBA(in, out, numPixels);
}

inline void G3D_DEPRECATED RGBtoBGR(
    const uint8*            in,
    uint8*                  out,
    int                     numPixels) {
    GImage::RGBtoBGR(in, out, numPixels);
}

/**
 Win32 32-bit HDC format.
 */
inline void G3D_DEPRECATED RGBtoBGRA(
    const uint8*            in,
    uint8*                  out,
    int                     numPixels) {
    GImage::RGBtoBGRA(in, out, numPixels);
}

inline void G3D_DEPRECATED RGBtoARGB(
    const uint8*            in,
    uint8*                  out,
    int                     numPixels) {
    GImage::RGBtoARGB(in, out, numPixels);
}


/**
 Uses the red channel of the second image as an alpha channel.
 */
inline void G3D_DEPRECATED RGBxRGBtoRGBA(
    const uint8*            colorRGB,
    const uint8*            alphaRGB,
    uint8*                  out,
    int                     numPixels) {
    GImage::RGBxRGBtoRGBA(colorRGB, alphaRGB, out, numPixels);
}
    
/**
 Flips the image along the vertical axis.
 Safe for in == out.
 */
inline void G3D_DEPRECATED flipRGBVertical(
    const uint8*            in,
    uint8*                  out,
    int                     width,
    int                     height) {
    GImage::flipRGBVertical(in, out, width, height);
}

inline void G3D_DEPRECATED flipRGBAVertical(
    const uint8*            in,
    uint8*                  out,
    int                     width,
    int                     height) {
    GImage::flipRGBAVertical(in, out, width, height);
}

/**
 Given a tangent space bump map, computes a new image where the
 RGB channels are a tangent space normal map and the alpha channel
 is the original bump map.  Assumes the input image is tileable.

 In the resulting image, x = red = tangent, y = green = binormal, and z = blue = normal. 

 Particularly useful as part of the idiom:
 <PRE>
 	GImage normal;
	computeNormalMap(GImage(filename), normal);
	return Texture::fromGImage(filename, normal);
 </PRE>

  @param lowPassBump If true, a 9x9 filter of 1's is used to low-pass filter the elevations,
       which produces better results for parallax mapping.

  @param scaleHeightByNz After computing normals, scale the height by |N.z|, a trick that
       reduces texture swim in steep areas for parallax mapping.

  @cite ATI demo
 */
inline void G3D_DEPRECATED computeNormalMap(
    const class GImage& bump, 
    class GImage& normal,
    bool lowPassBump = false,
    bool scaleHeightByNz = false) {
    GImage::computeNormalMap(bump, normal, lowPassBump, scaleHeightByNz);
}

/**
  Bayer demosaicing using the filter proposed in 

HIGH-QUALITY LINEAR INTERPOLATION FOR DEMOSAICING OF BAYER-PATTERNED COLOR IMAGES
Henrique S. Malvar, Li-wei He, and Ross Cutler

  The filter wraps at the image boundaries.
*/
inline void G3D_DEPRECATED BAYER_G8B8_R8G8_to_R8G8B8_MHC(int w, int h, const uint8* in, uint8* _out) {
    GImage::BAYER_G8B8_R8G8_to_R8G8B8_MHC(w, h, in, _out);
}



/** Fast conversion; the output has 1/2 the size of the input in each direction. Assumes in != out.
 See G3D::BAYER_G8B8_R8G8_to_R8G8B8_MHC for a much better result. */
inline void G3D_DEPRECATED BAYER_G8B8_R8G8_to_Quarter_R8G8B8(int inWidth, int inHeight, const uint8* in, uint8* out) {
    GImage::BAYER_G8B8_R8G8_to_Quarter_R8G8B8(inWidth, inHeight, in, out);
}

/** Attempt to undo fast conversion of G3D::BAYER_G8B8_R8G8_to_Quarter_R8G8B8; 
    the green channel will lose data. Assumes in != out 
    The input should have size 3 * inWidth * inHeight.  The output should have size
    2 * inWidth * 2 * inHeight.
*/
inline void G3D_DEPRECATED Quarter_R8G8B8_to_BAYER_G8B8_R8G8(int inWidth, int inHeight, const uint8* in, uint8* out) {
    GImage::Quarter_R8G8B8_to_BAYER_G8B8_R8G8(inWidth, inHeight, in, out);
}


/**
 @deprecated
 */
typedef GImage CImage;

}

#endif

